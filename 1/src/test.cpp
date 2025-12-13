#include "bits/stdc++.h"

using namespace std;

#define fastio ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)

typedef long long ll;
typedef unsigned long long ull;

typedef pair<int,int> ii;
typedef pair<ii,ii> iiii;
typedef pair<ll,int> li;
typedef pair<int,ii> iii;
typedef pair<ii,int> iii2;
typedef pair<ll,ll> pll;
typedef pair<int,ll> il;

//ll mod = 998244353ll;

const ll large = 1000000000000000000LL;
const int int_large = 1e9;

int __test_case = 1;


// prepare----------------------------------------------------------------


void __prepare(){
//	cin>>__test_case;

}


// begin----------------------------------------------------------------


const int N = 5000007;
const int bN = 23;
const int p = 998244353, gg = 3, ig = 332738118, img = 86583718;
const int mod = 998244353;

template <typename T>void read(T &x)
{
    x = 0;
    register int f = 1;
    register char ch = getchar();
    while(ch < '0' || ch > '9') {if(ch == '-')f = -1;ch = getchar();}
    while(ch >= '0' && ch <= '9') {x = x * 10 + ch - '0';ch = getchar();}
    x *= f;
}

int qpow(int a, ll b)
{
    int res = 1;
    while(b) {
        if(b & 1) res = 1ll * res * a % mod;
        a = 1ll * a * a % mod;
        b >>= 1;
    }
    return res;
}

namespace Poly
{
    #define mul(x, y) (1ll * x * y >= mod ? 1ll * x * y % mod : 1ll * x * y)
    #define minus(x, y) (1ll * x - y < 0 ? 1ll * x - y + mod : 1ll * x - y)
    #define plus(x, y) (1ll * x + y >= mod ? 1ll * x + y - mod : 1ll * x + y)
    #define ck(x) (x >= mod ? x - mod : x)//取模运算太慢了

    typedef vector<int> poly;
    const int G = 3;//根据具体的模数而定，原根可不一定不一样！！！
    //一般模数的原根为 2 3 5 7 10 6
    const int inv_G = qpow(G, mod - 2);
    int RR[N], deer[2][bN][N], inv[N];

    void init(const int t) {//预处理出来NTT里需要的w和wn，砍掉了一个log的时间
        for(int p = 1; p <= t; ++ p) {
            int buf1 = qpow(G, (mod - 1) / (1 << p));
            int buf0 = qpow(inv_G, (mod - 1) / (1 << p));
            deer[0][p][0] = deer[1][p][0] = 1;
            for(int i = 1; i < (1 << p); ++ i) {
                deer[0][p][i] = 1ll * deer[0][p][i - 1] * buf0 % mod;//逆
                deer[1][p][i] = 1ll * deer[1][p][i - 1] * buf1 % mod;
            }
        }
        inv[1] = 1;
        for(int i = 2; i <= (1 << t); ++ i)
            inv[i] = 1ll * inv[mod % i] * (mod - mod / i) % mod;
    }

    int NTT_init(int n) {//快速数论变换预处理
        int limit = 1, L = 0;
        while(limit <= n) limit <<= 1, L ++ ;
        for(int i = 0; i < limit; ++ i)
            RR[i] = (RR[i >> 1] >> 1) | ((i & 1) << (L - 1));
        return limit;
    }

    void NTT(poly &A, int type, int limit) {//快速数论变换
        A.resize(limit);
        for(int i = 0; i < limit; ++ i)
            if(i < RR[i])
                swap(A[i], A[RR[i]]);
        for(int mid = 2, j = 1; mid <= limit; mid <<= 1, ++ j) {
            int len = mid >> 1;
            for(int pos = 0; pos < limit; pos += mid) {
                int *wn = deer[type][j];
                for(int i = pos; i < pos + len; ++ i, ++ wn) {
                    int tmp = 1ll * (*wn) * A[i + len] % mod;
                    A[i + len] = ck(A[i] - tmp + mod);
                    A[i] = ck(A[i] + tmp);
                }
            }
        }
        if(type == 0) {
            for(int i = 0; i < limit; ++ i)
                A[i] = 1ll * A[i] * inv[limit] % mod;
        }
    }

    poly poly_mul(poly A, poly B) {//多项式乘法
        int deg = A.size() + B.size() - 1;
        int limit = NTT_init(deg);
        poly C(limit);
        NTT(A, 1, limit);
        NTT(B, 1, limit);
        for(int i = 0; i < limit; ++ i)
            C[i] = 1ll * A[i] * B[i] % mod;
        NTT(C, 0, limit);
        C.resize(deg);
        return C;
    }

    poly poly_inv(poly &f, int deg) {//多项式求逆
        if(deg == 1)
            return poly(1, qpow(f[0], mod - 2));

        poly A(f.begin(), f.begin() + deg);
        poly B = poly_inv(f, (deg + 1) >> 1);
        int limit = NTT_init(deg << 1);
        NTT(A, 1, limit), NTT(B, 1, limit);
        for(int i = 0; i < limit; ++ i)
            A[i] = B[i] * (2 - 1ll * A[i] * B[i] % mod + mod) % mod;
        NTT(A, 0, limit);
        A.resize(deg);
        return A;
    }

    poly poly_dev(poly f) {//多项式求导
        int n = f.size();
        for(int i = 1; i < n; ++ i) f[i - 1] = 1ll * f[i] * i % mod;
        return f.resize(n - 1), f;//f[0] = 0，这里直接扔了,从1开始
    }

    poly poly_idev(poly f) {//多项式求积分
        int n = f.size();
        for(int i = n - 1; i ; -- i) f[i] = 1ll * f[i - 1] * inv[i] % mod;
        return f[0] = 0, f;
    }

    poly poly_ln(poly f, int deg) {//多项式求对数
        poly A = poly_idev(poly_mul(poly_dev(f), poly_inv(f, deg)));
        return A.resize(deg), A;
    }

    poly poly_exp(poly &f, int deg) {//多项式求指数
        if(deg == 1)
            return poly(1, 1);

        poly B = poly_exp(f, (deg + 1) >> 1);
        B.resize(deg);
        poly lnB = poly_ln(B, deg);
        for(int i = 0; i < deg; ++ i)
            lnB[i] = ck(f[i] - lnB[i] + mod);

        int limit = NTT_init(deg << 1);//n -> n^2
        NTT(B, 1, limit), NTT(lnB, 1, limit);
        for(int i = 0; i < limit; ++ i)
            B[i] = 1ll * B[i] * (1 + lnB[i]) % mod;
        NTT(B, 0, limit);
        B.resize(deg);
        return B;
    }

    poly poly_sqrt(poly &f, int deg) {//多项式开方
        if(deg == 1) return poly(1, 1);
        poly A(f.begin(), f.begin() + deg);
        poly B = poly_sqrt(f, (deg + 1) >> 1);
        poly IB = poly_inv(B, deg);
        int limit = NTT_init(deg << 1);
        NTT(A, 1, limit), NTT(IB, 1, limit);
        for(int i = 0; i < limit; ++ i)
            A[i] = 1ll * A[i] * IB[i] % mod;
        NTT(A, 0, limit);
        for(int i =0; i < deg; ++ i)
            A[i] = 1ll * (A[i] + B[i]) * inv[2] % mod;
        A.resize(deg);
        return A;
    }

    poly poly_pow(poly f, int k) {//多项式快速幂
        f = poly_ln(f, f.size());
        for(auto &x : f) x = 1ll * x * k % mod;
        return poly_exp(f, f.size());
    }

    poly poly_rev(const poly &f, int deg) {//多项式翻转
		poly g(deg);
		int n = f.size();
		for(int i = 0; i < deg; ++ i) {
			if (n - 1 - i >= 0 && n - 1 - i < n)
				g[i] = f[n - 1 - i];
			else
				g[i] = 0;
		}
		return g;
	}

    //多项式相除求商和余数
	pair<poly, poly> poly_div(poly F, poly G) {
		// F(x) = Q(x)G(x) + R(x)
		int n = F.size(), m = G.size();
		if (n < m) { // 被除式次数小于除式次数
			return {poly(1, 0), F}; // 商为 0，余为 F
		}

		// Q(x) 的次数 k = n - m
		int k = n - m;
		int deg_k_plus_1 = k + 1;

		// 1. 翻转 F 和 G，并截取 G^R 到 k+1 次
		poly FR = poly_rev(F, n); // F^R, deg: n-1
		poly GR = poly_rev(G, m); // G^R, deg: m-1
		GR.resize(deg_k_plus_1);

		// 2. 求 (G^R)^-1 mod x^(k+1)
		poly inv_GR = poly_inv(GR, deg_k_plus_1);

		// 3. 计算 Q^R(x) = F^R(x) * (G^R(x))^-1 mod x^(k+1)
		// 注意：F^R 需要 resize 到足够的长度 (2*k+2) 才能进行乘法
		// 但根据原理，我们只需要乘积 mod x^(k+1) 的部分。
		// 将 FR 截断到 k+1 次就够了，因为乘积的高次项会被模掉。
		FR.resize(deg_k_plus_1);
		poly QR = poly_mul(FR, inv_GR);
		QR.resize(deg_k_plus_1);

		// 4. 翻转得到商式 Q(x)
		// Q(x) 次数为 k，所以 Q^R(x) 的系数应取前 k+1 项 (0到k次)
		poly Q = poly_rev(QR, deg_k_plus_1); // Q(x) 长度为 k+1, deg: k

		// 5. 计算余式 R(x) = F(x) - Q(x)G(x)
		poly QG = poly_mul(Q, G);
		int R_deg = m - 1;
		poly R(R_deg + 1);

		// 循环计算 R 的每一项系数: R[i] = F[i] - (QG)[i]
		for (int i = 0; i <= R_deg; ++ i) {
			int QG_i = (i < (int)QG.size()) ? QG[i] : 0;
			R[i] = minus(F[i], QG_i);
		}

		return {Q, R};
	}

    poly poly_cos(poly f, int deg) {//多项式三角函数（cos）
        poly A(f.begin(), f.begin() + deg);
        poly B(deg), C(deg);
        for(int i = 0; i < deg; ++ i)
            A[i] = 1ll * A[i] * img % mod;

        B = poly_exp(A, deg);
        C = poly_inv(B, deg);
        int inv2 = qpow(2, mod - 2);
        for(int i = 0; i < deg; ++ i)
            A[i] = 1ll * (1ll * B[i] + C[i]) % mod * inv2 % mod;
        return A;
    }

    poly poly_sin(poly f, int deg) {//多项式三角函数（sin）
        poly A(f.begin(), f.begin() + deg);
        poly B(deg), C(deg);
        for(int i = 0; i < deg; ++ i)
            A[i] = 1ll * A[i] * img % mod;

        B = poly_exp(A, deg);
        C = poly_inv(B, deg);
        int inv2i = qpow(img << 1, mod - 2);
        for(int i = 0; i < deg; ++ i)
            A[i] = 1ll * (1ll * B[i] - C[i] + mod) % mod * inv2i % mod;
        return A;
    }

    poly poly_arcsin(poly f, int deg) {
        poly A(f.size()), B(f.size()), C(f.size());
        A = poly_dev(f);
        B = poly_mul(f, f);
        for(int i = 0; i < deg; ++ i)
            B[i] = minus(mod, B[i]);
        B[0] = plus(B[0], 1);
        C = poly_sqrt(B, deg);
        C = poly_inv(C, deg);
        C = poly_mul(A, C);
        C = poly_idev(C);
        return C;
    }

    poly poly_arctan(poly f, int deg) {
        poly A(f.size()), B(f.size()), C(f.size());
        A = poly_dev(f);
        B = poly_mul(f, f);
        B[0] = plus(B[0], 1);
        C = poly_inv(B, deg);
        C = poly_mul(A, C);
        C = poly_idev(C);
        return C;
    }

    // -----------------------------------------------------------
	// 多项式多点求值所需辅助函数
	// -----------------------------------------------------------

	// 辅助结构体：用于存储模数树的节点
	vector<poly> M;// 存储模数树 M[i]

	// 1. 构造模数树
	void build_tree(const vector<int> &x, int i, int l, int r) {
		if (l == r) {
			// 叶子节点: M[i](x) = x - x[l]
			M[i].resize(2);
			M[i][0] = minus(mod, x[l]); // -x[l]
			M[i][1] = 1;                 // x
			return;
		}

		int mid = (l + r) >> 1;
		build_tree(x, i << 1, l, mid);
		build_tree(x, i << 1 | 1, mid + 1, r);
		// 内部节点: M[i] = M[i*2] * M[i*2+1]
		M[i] = poly_mul(M[i << 1], M[i << 1 | 1]);
	}

	// 2. 多项式取模（直接调用 poly_div 并返回余数 R）
	poly poly_mod(poly F, poly G) {
		// F(x) mod G(x) = R(x)
		int n = F.size(), m = G.size();
		if (n < m) return F; // deg(F) < deg(G), 余数即为 F

		// 调用 poly_div，我们只需要余数 R
		// poly_div 返回 {Q, R}
		return poly_div(F, G).second;
	}

	// 3. 分治求值
	void solve_eval(poly A, int i, int l, int r, vector<int> &res) {
		if (l == r) {
			// 叶子节点: 结果为 A(x) mod (x - x_l) 的常数项
			// A(x) mod (x - x_l) = R_0 (常数)
			res[l] = A[0];
			return;
		}

		int mid = (l + r) >> 1;

		// A_left(x) = A(x) mod M[i*2](x)
		poly A_left = poly_mod(A, M[i << 1]);
		solve_eval(A_left, i << 1, l, mid, res);

		// A_right(x) = A(x) mod M[i*2+1](x)
		poly A_right = poly_mod(A, M[i << 1 | 1]);
		solve_eval(A_right, i << 1 | 1, mid + 1, r, res);
	}

	// -----------------------------------------------------------
	// 多项式多点求值主函数
	// -----------------------------------------------------------
	// F: 待求值的多项式
	// x: 求值点集合
	vector<int> poly_eval(poly F, const vector<int> &x) {
		int m = x.size();
		if (m == 0) return {};

		// 1. 构造模数树
		M.resize(4 * m); // 树的节点数量约为 4*m
		build_tree(x, 1, 0, m - 1);

		// 2. 截断 F(x): A'(x) = F(x) mod M[1, m](x)
		// M[1] 是整棵树的根，即 M[1, m](x)
		F = poly_mod(F, M[1]);

		// 3. 分治求值
		vector<int> res(m);
		solve_eval(F, 1, 0, m - 1, res);

		return res;
	}

	// -----------------------------------------------------------
	// 多项式快速插值所需辅助函数
	// -----------------------------------------------------------

	// 4. 分治插值
	// x: 节点 x 坐标, y: 节点 y 坐标
	// i: 模数树节点索引, l, r: 节点对应 x 的范围
	poly solve_interp(const vector<int> &x, const vector<int> &y,
					  const vector<int> &inv_Mx, int i, int l, int r) {

		if (l == r) {
			// 叶子节点：返回常数多项式 P(x) = y[l] / M'(x[l])
			poly P(1);
			// P[0] = y[l] * inv_Mx[l]
			P[0] = 1ll * y[l] * inv_Mx[l] % mod;
			return P;
		}

		int mid = (l + r) >> 1;

		// 递归计算左侧插值结果 P_L(x) 和右侧插值结果 P_R(x)
		poly P_L = solve_interp(x, y, inv_Mx, i << 1, l, mid);
		poly P_R = solve_interp(x, y, inv_Mx, i << 1 | 1, mid + 1, r);

		// 合并：P(x) = P_L(x) * M_R(x) + P_R(x) * M_L(x)
		// M_L(x) = M[i*2], M_R(x) = M[i*2+1]

		// P_L(x) * M_R(x)
		poly term1 = poly_mul(P_L, M[i << 1 | 1]);

		// P_R(x) * M_L(x)
		poly term2 = poly_mul(P_R, M[i << 1]);

		// P(x) = term1 + term2
		int deg_P = max(term1.size(), term2.size());
		poly P(deg_P);

		for (int j = 0; j < deg_P; ++ j) {
			int t1 = (j < term1.size()) ? term1[j] : 0;
			int t2 = (j < term2.size()) ? term2[j] : 0;
			P[j] = plus(t1, t2);
		}

		return P;
	}

	// -----------------------------------------------------------
	// 多项式快速插值主函数
	// -----------------------------------------------------------
	// x: 插值点的 x 坐标, y: 插值点的 y 坐标 (必须 |x| = |y|)
	poly poly_fast_interp(const vector<int> &x, const vector<int> &y) {
		int n = x.size();
		if (n == 0) return poly(1, 0);

		// 1. 构造模数树 M[i]
		// M 已经在 poly_eval 中定义并使用了，这里沿用 M
		M.resize(4 * n);
		build_tree(x, 1, 0, n - 1);

		// 2. 计算 M(x) = M[1, n](x)
		poly Mx = M[1];

		// 3. 计算 M'(x) (求导)
		poly Mx_dev = poly_dev(Mx);

		// 4. 多点求值计算 M'(x_i)
		// 注意：Mx_dev 的长度为 n (deg: n-1)，x 的长度为 n
		vector<int> Mx_dev_at_xi = poly_eval(Mx_dev, x);

		// 5. 计算 M'(x_i) 的逆 (1 / M'(x_i))
		vector<int> inv_Mx(n);
		for (int i = 0; i < n; ++ i) {
			// 确保 M'(x_i) 不为 0，否则插值点不唯一！
			if (Mx_dev_at_xi[i] == 0) {
				// 应该抛出异常或返回错误，因为插值点有重复
				// 在模 mod 意义下，M'(x_i)=0 表示 x_i 是 M(x) 的重根
			}
			inv_Mx[i] = qpow(Mx_dev_at_xi[i], mod - 2);
		}

		// 6. 分治计算 P(x)
		poly P = solve_interp(x, y, inv_Mx, 1, 0, n - 1);

		return P;
	}

	// -----------------------------------------------------------
	// Bostan–Mori 算法辅助函数
	// -----------------------------------------------------------

	// 多项式抽取：type=0 偶数次项系数, type=1 奇数次项系数
	poly poly_extract(const poly &f, int type) {
		poly res;
		int n = f.size();
		for (int i = type; i < n; i += 2) {
			res.push_back(f[i]);
		}
		if (res.empty()) res.push_back(0); // 保证不为空，避免 NTT 出错
		return res;
	}

	// -----------------------------------------------------------
	// Bostan–Mori 算法主函数
	// -----------------------------------------------------------
	// 计算 [x^k] P(x) / Q(x)
	int bostan_mori(poly P, poly Q, long long k) {
		while (k > 0) {
			// 1. 预处理 Q 的奇偶部分
			poly Qe = poly_extract(Q, 0); // Q_e(x)
			poly Qo = poly_extract(Q, 1); // Q_o(x)

			// 2. 计算新分母 Q_new(x) = Q_e^2(x) - x Q_o^2(x)
			// 这一步与 k 的奇偶性无关，始终执行
			poly Qe_sq = poly_mul(Qe, Qe);
			poly Qo_sq = poly_mul(Qo, Qo);

			int deg_Q_new = max(Qe_sq.size(), Qo_sq.size() + 1);
			Q.resize(deg_Q_new);
			for(int i = 0; i < deg_Q_new; ++ i) {
				int term1 = (i < (int)Qe_sq.size()) ? Qe_sq[i] : 0;
				// 注意：x * Qo^2(x) 意味着系数向高位移动一位，即取 i-1 项
				int term2 = (i > 0 && i - 1 < (int)Qo_sq.size()) ? Qo_sq[i - 1] : 0;
				Q[i] = minus(term1, term2);
			}

			// 3. 预处理 P 的奇偶部分
			poly Pe = poly_extract(P, 0);
			poly Po = poly_extract(P, 1);

			// 4. 计算新分子 P_new(x)，根据 k 的奇偶性选择不同的计算路径（优化掉一半的 FFT）
			if (k & 1) {
				// CASE: k 是奇数
				// 公式：P_new(x) = P_o(x)Q_e(x) - P_e(x)Q_o(x)
				poly Po_Qe = poly_mul(Po, Qe);
				poly Pe_Qo = poly_mul(Pe, Qo);

				int deg_P_new = max(Po_Qe.size(), Pe_Qo.size());
				P.resize(deg_P_new);
				for(int i = 0; i < deg_P_new; ++ i) {
					int t1 = (i < (int)Po_Qe.size()) ? Po_Qe[i] : 0;
					int t2 = (i < (int)Pe_Qo.size()) ? Pe_Qo[i] : 0;
					P[i] = minus(t1, t2);
				}
			} else {
				// CASE: k 是偶数
				// 公式：P_new(x) = P_e(x)Q_e(x) - x P_o(x)Q_o(x)
				poly Pe_Qe = poly_mul(Pe, Qe);
				poly Po_Qo = poly_mul(Po, Qo);

				int deg_P_new = max(Pe_Qe.size(), Po_Qo.size() + 1);
				P.resize(deg_P_new);
				for(int i = 0; i < deg_P_new; ++ i) {
					int t1 = (i < (int)Pe_Qe.size()) ? Pe_Qe[i] : 0;
					// 注意：x * (PoQo) 意味着系数移动一位，取 i-1
					int t2 = (i > 0 && i - 1 < (int)Po_Qo.size()) ? Po_Qo[i - 1] : 0;
					P[i] = minus(t1, t2);
				}
			}

			// 5. 迭代 k
			k >>= 1;

			// 6. 去除高位无用的 0，减小下一轮 NTT 的长度
			while(P.size() > 1 && P.back() == 0) P.pop_back();
			while(Q.size() > 1 && Q.back() == 0) Q.pop_back();
		}

		// k=0 时，答案是常数项之商 P(0)/Q(0)
		// 注意 Q[0] 必须存在。如果 Q[0]=0，且 P[0]!=0 则无解；若都是 0 则需洛必达（一般题目保证 Q[0]!=0）
		return 1ll * P[0] * qpow(Q[0], mod - 2) % mod;
	}

	// -----------------------------------------------------------
	// 求线性递推数列第 k 项
	// -----------------------------------------------------------
	// c: 递推系数 [c_1, c_2, ..., c_m] ( a_n = c_1*a_{n-1} + ... )
	// a: 初值 [a_0, a_1, ..., a_{m-1}]
	// k: 要求的项数下标 (从 0 开始)
	int linear_recurrence(vector<int> c, vector<int> a, long long k) {
		int m = c.size(); // 递推阶数

		// 1. 构造分母 Q(x) = 1 - c_1 x - c_2 x^2 ...
		poly Q(m + 1);
		Q[0] = 1;
		for (int i = 0; i < m; ++ i) {
			// 注意符号：是减去系数
			Q[i + 1] = minus(0, c[i]);
		}

		// 2. 构造分子 P(x) = A(x)Q(x) mod x^m
		// A(x) 是初值构成的多项式
		poly A = a; // A 的度数是 m-1

		// 我们只需要前 m 项，所以乘法后 resize 即可
		// 实际上 P 的次数最高为 m-1
		poly P = poly_mul(A, Q);
		if (P.size() > m) P.resize(m);

		// 3. 调用 Bostan-Mori
		return bostan_mori(P, Q, k);
	}
}

void solve(){
	Poly::init(21);

	int n,k;
	cin>>n>>k;
	vector<int> a(n,0),c(n,0);
	for(int i=0;i<n;i++) cin>>a[i];
	for(int i=0;i<n;i++) cin>>c[i];

	cout<<Poly::linear_recurrence(c,a,k)<<endl;



}



int main(){
	fastio;

	__prepare();

	while(__test_case--){
		solve();
	}

	return 0;
}
