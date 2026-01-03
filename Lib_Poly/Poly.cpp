//#pragma GCC optimize(2)
#include "bits/stdc++.h"
using namespace std;
typedef long long ll;


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
	// #define mul(x, y) (1ll * x * y >= mod ? 1ll * x * y % mod : 1ll * x * y)
    inline int mul(int x, int y) {
        return 1ll * x * y >= mod ? 1ll * x * y % mod : 1ll * x * y;
    }

    // #define minus(x, y) (1ll * x - y < 0 ? 1ll * x - y + mod : 1ll * x - y)
    inline int minus(int x, int y) {
        return x - y < 0 ? x - y + mod : x - y;
    }

    // #define plus(x, y) (1ll * x + y >= mod ? 1ll * x + y - mod : 1ll * x + y)
    inline int plus(int x, int y) {
        return x + y >= mod ? x + y - mod : x + y;
    }

    // #define ck(x) (x >= mod ? x - mod : x)
    inline int ck(int x) {
        return x >= mod ? x - mod : x;
    }

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

    poly poly_mul(const poly &A, const poly &B) {
        // 内部逻辑不变，但因为是引用，不能直接 resize A 或 B
        // 你需要创建新的临时变量
        int deg = A.size() + B.size() - 1;
        int limit = NTT_init(deg);

        poly FA(A.begin(), A.end()); FA.resize(limit); // 显式拷贝并 resize
        poly FB(B.begin(), B.end()); FB.resize(limit);

        NTT(FA, 1, limit);
        NTT(FB, 1, limit);
        poly FC(limit);
        for(int i = 0; i < limit; ++ i)
            FC[i] = 1ll * FA[i] * FB[i] % mod;
        NTT(FC, 0, limit);
        FC.resize(deg);
        return FC;
    }

    poly poly_inv(poly &f, int deg) {//多项式求逆
        if(deg == 1)
            return poly(1, qpow(f[0], mod - 2));

        poly A(f.begin(), f.begin() + deg);
        A.resize(deg,0);
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

	// --- 辅助多项式复合
	// --- 请添加到 Poly 命名空间的变量声明部分 ---
	int fac[N], ifac[N];

	// --- 请更新 init 函数，或者添加这个辅助函数 ---
	void init_fac(int n) {
		fac[0] = 1;
		for (int i = 1; i <= n; ++i) fac[i] = 1ll * fac[i - 1] * i % mod;
		ifac[n] = qpow(fac[n], mod - 2);
		for (int i = n - 1; i >= 0; --i) ifac[i] = 1ll * ifac[i + 1] * (i + 1) % mod;
	}

	// -----------------------------------------------------------
	// 多项式复合辅助函数：泰勒平移 f(x + c)
	// -----------------------------------------------------------
	// 原理：f(x + c) = sum ( f[i] * i! * c^(i-j) / (i-j)! * x^j / j! )
	// 这是一个卷积形式
	poly poly_shift(poly f, int c) {
		int n = f.size();
		// 记得调用 init_fac 预处理阶乘！
		// 如果 c = 0，直接返回
		if (c == 0) return f;

		poly A(n), B(n);
		// 构造 A[i] = f[i] * i!
		for (int i = 0; i < n; ++i)
			A[i] = 1ll * f[i] * fac[i] % mod;

		// 构造 B[i] = c^i / i!
		// 注意：卷积是从大到小匹配，所以我们要翻转 A，
		// 或者构造卷积形式：sum A[j+k] * B[k]
		// 这里采用翻转 A 的方式：
		// [x^j] Result = (1/j!) * sum_{i=j}^n (f[i]*i!) * (c^(i-j)/(i-j)!)
		// 令 A'[n-1-i] = f[i]*i!, B[k] = c^k/k!
		// 卷积后取相关项

		reverse(A.begin(), A.end());

		int pc = 1;
		for (int i = 0; i < n; ++i) {
			B[i] = 1ll * pc * ifac[i] % mod;
			pc = 1ll * pc * c % mod;
		}

		poly Res = poly_mul(A, B); // 卷积
		Res.resize(n); // 只需要前 n 项（实际上是卷积结果的高位部分）
		reverse(Res.begin(), Res.end()); // 翻转回来

		// 最后乘上 1/j!
		for (int i = 0; i < n; ++i)
			Res[i] = 1ll * Res[i] * ifac[i] % mod;

		return Res;
	}

	// -----------------------------------------------------------
	// 多项式复合 (优化版 O(N log^2 N))
	// -----------------------------------------------------------

	// 辅助：限制长度的乘法，避免不必要的 FFT 计算
	poly poly_mul_len(const poly &A, const poly &B, int limit) {
		if (A.empty() || B.empty() || limit <= 0) return {};
		// 截断输入，只取有效部分
		poly FA(A.begin(), A.begin() + min((int)A.size(), limit));
		poly FB(B.begin(), B.begin() + min((int)B.size(), limit));

		poly res = poly_mul(FA, FB);
		if (res.size() > limit) res.resize(limit);
		return res;
	}

	// 分治核心
	// f: 当前层级的系数
	// h_powers: 预处理好的 H 的幂次 H^1, H^2, H^4... (其中 H = G/x)
	// k: 当前层级 (对应 shift = 2^k)
	// limit_deg: 目标模数次数
	poly solve_comp_opt(const poly &f, const vector<poly> &h_powers, int k, int limit_deg) {
		if (f.size() <= 1) return f;

		int n = f.size();
		int mid = n >> 1;

		// 1. 分割 f 为左半部分和右半部分
		poly f_left(f.begin(), f.begin() + mid);
		poly f_right(f.begin() + mid, f.end());

		// 2. 递归求解
		poly res_left = solve_comp_opt(f_left, h_powers, k - 1, limit_deg);

		// 关键优化：右半部分不仅要递归，还要乘上 G^mid
		// G^mid = x^mid * H^mid
		// 所以我们只需要计算 mod (limit_deg - mid)
		int shift = 1 << (k - 1); // 当前层的 mid 跨度，即 2^(k-1)

		if (shift >= limit_deg) {
			// 如果移位超过限制，右半部分贡献为 0，直接返回左半部分
			return res_left;
		}

		poly res_right = solve_comp_opt(f_right, h_powers, k - 1, limit_deg - shift);

		// 3. 合并：res = res_left + (res_right * H^mid) << mid
		// 取出 H^mid (在 h_powers[k-1] 中)
		poly H_pow = h_powers[k - 1];

		// 计算乘积，注意长度限制减少了 shift
		poly term_right = poly_mul_len(res_right, H_pow, limit_deg - shift);

		// 加法合并
		if (res_left.size() < limit_deg) res_left.resize(limit_deg);

		for (int i = 0; i < term_right.size(); ++i) {
			if (i + shift < limit_deg) {
				res_left[i + shift] = plus(res_left[i + shift], term_right[i]);
			}
		}

		return res_left;
	}

	// 主函数
	poly poly_comp(poly f, poly g, int n) {
		f.resize(n);
		g.resize(n);

		// 处理 g[0] != 0 的情况 (泰勒平移)
		if (g.size() > 0 && g[0] != 0) {
			 // 必须确保已调用 init_fac()
			f = poly_shift(f, g[0]);
			g[0] = 0;
		}

		// 将 f 补齐到 2 的幂次，方便分治
		int m = 1;
		while (m < n) m <<= 1;
		f.resize(m, 0);

		// 构造 H(x) = G(x) / x
		poly h = g;
		if (h.size() > 0) h.erase(h.begin()); // 移除常数项 0
		else h = {0};

		// 预处理 H 的幂次：H^1, H^2, H^4...
		// 此时 H 的长度只需要 n
		int log_m = 0;
		while ((1 << log_m) < m) log_m++;

		vector<poly> h_powers(log_m);
		if (log_m > 0) {
			h_powers[0] = h; // H^1
			if (h_powers[0].size() > n) h_powers[0].resize(n);

			for (int i = 1; i < log_m; ++i) {
				h_powers[i] = poly_mul_len(h_powers[i - 1], h_powers[i - 1], n);
			}
		}

		poly res = solve_comp_opt(f, h_powers, log_m, n);
		res.resize(n);
		return res;
	}

	// -----------------------------------------------------------
	// 多项式复合逆 (极速优化版)
	// -----------------------------------------------------------
	// 复杂度：O(N log^2 N)，但常数极小
	poly poly_inv_comp(poly F, int n) {
		// 1. 基础校验
		if (F.empty() || F[0] != 0) return {};
		if (F.size() > 1 && F[1] == 0) return {};

		// 2. 初始状态：G(x) = x / F'(0) mod x^2
		poly G(2);
		G[0] = 0;
		int inv_f1 = qpow(F[1], mod - 2);
		G[1] = inv_f1;

		// 3. 倍增迭代
		// len: 当前已求出的解的有效长度
		int len = 2;
		while (len < n) {
			// 目标长度 limit = 2 * len
			int limit = len << 1;

			// --- 步骤 A: 准备 F ---
			// F 只需要截取到 limit，超过 limit 的项对模 limit 的结果无影响
			poly F_cut = F;
			if (F_cut.size() > limit) F_cut.resize(limit);

			// G_cur 用于代入，resize 到 limit (后面补0)
			poly G_cur = G;
			if (G_cur.size() < limit) G_cur.resize(limit);

			// --- 步骤 B: 计算复合 A = F(G(x)) mod x^limit ---
			// 这是全算法最耗时的一步 O(len log^2 len)
			poly A = poly_comp(F_cut, G_cur, limit);

			// --- 步骤 C: 计算分母 (F(G))' 的逆 ---
			// [关键优化 1]：分母只需要计算模 x^len 的逆！
			// 因为分子 (A-x) 最低位是 x^len，所以分母高位不影响结果的 mod x^limit
			poly A_dev = poly_dev(A);
			if (A_dev.size() > len) A_dev.resize(len); // 强制截断到 len

			poly den_inv = poly_inv(A_dev, len); // 只做 len 长求逆，速度快一倍

			// --- 步骤 D: 计算分子 (F(G) - x) * G' ---

			// 1. 计算残差 R = A - x
			// 我们知道前 len 项肯定是 0 (或是非常接近 0 的极小误差)，
			// 实际上我们只需要 A 的 [len, limit) 部分
			if (A.size() > 1) A[1] = minus(A[1], 1);
			else { A.resize(2, 0); A[1] = minus(0, 1); }

			// 2. 计算 G'
			// [关键优化 2]：G' 也只需要前 len 项参与计算 delta 的高位
			poly G_dev = poly_dev(G_cur);
			if (G_dev.size() > len) G_dev.resize(len);

			// 3. 分子 num = (A-x) * G'
			// 注意：A-x 的前 len 项理论上是 0。
			// 为了利用这一点，我们可以手动 slice 或者直接乘。
			// 直接乘比较安全，但我们可以限制结果长度。
			poly num = poly_mul(A, G_dev);
			// 我们只需要 num 的 [len, limit) 部分，但 poly_mul 算出了 [0, 2*len)
			// 这里的低位部分应该是 0

			// --- 步骤 E: 计算增量 Delta 并更新 ---
			// Delta = num * den_inv
			// 同样，我们只需要 Delta 的 [len, limit) 部分
			// 这一步乘法可以用类似分治乘法的逻辑优化，但直接乘也行，主要是 den_inv 变短了
			poly delta = poly_mul(num, den_inv);

			// 更新 G = G - delta
			G.resize(limit);
			for (int i = len; i < limit; ++i) { // 只需要更新 [len, limit) 部分
				int d_val = (i < delta.size()) ? delta[i] : 0;
				G[i] = minus(G[i], d_val);
			}

			len <<= 1;
		}

		G.resize(n);
		return G;
	}

	// -----------------------------------------------------------
	// Berlekamp-Massey 算法
	// -----------------------------------------------------------
	// 输入: 数列的前若干项 val
	// 输出: 最短线性递推式的特征多项式 C(x) (连接多项式)
	// C(x) = 1 - c_1 x - c_2 x^2 ... - c_m x^m
	// 满足: \forall i >= m, \sum_{j=0}^m C_j * val_{i-j} = 0 (其中 C_0 = 1)
	poly berlekamp_massey(const vector<int> &val) {
		poly C, oldC;
		C.push_back(1);
		oldC.push_back(1);

		int fail_pos = -1; // 上一次失配的位置
		int val_in_fail = 0; // 上一次失配时的计算差值 (delta)

		for (int i = 0; i < (int)val.size(); ++i) {
			// 1. 计算当前差值 delta
			// delta = val[i] + \sum_{j=1}^{C.size()-1} C[j] * val[i-j]
			// 注意：这里 C 的定义包含 C[0]=1，且公式为 \sum C_j * val_{i-j} = 0
			// 所以 delta = \sum_{j=0}^{C.size()-1} C[j] * val[i-j]
			int delta = 0;
			for (int j = 0; j < (int)C.size(); ++j) {
				delta = (delta + 1ll * C[j] * val[i - j]) % mod;
			}

			if (delta == 0) continue; // 匹配成功，无需更新

			// 2. 匹配失败，需要修正 C
			// fail_pos 是上一次出错的位置 k
			// val_in_fail 是上一次的 delta_k
			// 构造因子 mul = -delta / delta_k
			int mul = 1ll * (mod - delta) * qpow(val_in_fail, mod - 2) % mod;

			// 构造新多项式 D(x) = C(x) + mul * x^{i - fail_pos} * oldC(x)
			// 因为我们要 copy C 到 newC，并在 newC 上叠加
			poly curC = C;
			int offset = i - fail_pos;

			// 动态扩展 curC 大小
			if (curC.size() < oldC.size() + offset)
				curC.resize(oldC.size() + offset);

			for (int j = 0; j < (int)oldC.size(); ++j) {
				curC[j + offset] = (curC[j + offset] + 1ll * mul * oldC[j]) % mod;
			}

			// 3. 更新 oldC 和 fail_pos
			// 如果 C 的长度增加了，通常意味着找到了更优的解，需要更新 fallback 信息
			if (C.size() < curC.size()) {
				oldC = C;
				fail_pos = i;
				val_in_fail = delta;
			}

			C = curC;
		}
		return C;
	}

	// -----------------------------------------------------------
	// 线性递推数列第 k 项 (自动推导版)
	// -----------------------------------------------------------
	// 输入: 数列的前若干项 raw_seq, 要求的项数 k
	// 逻辑:
	// 1. 运行 BM 算法得到分母 Q(x)
	// 2. 构造分子 P(x) = A(x)Q(x) mod x^m
	// 3. 运行 Bostan-Mori
	int kth_term_linear(const vector<int> &raw_seq, long long k) {
		if (raw_seq.empty()) return 0;
		if (k < (int)raw_seq.size()) return raw_seq[k];

		// 1. BM 求连接多项式 Q (即分母)
		// 注意：BM 返回的 C 实际上就是 Bostan-Mori 需要的分母 Q
		// Q = 1 - c_1 x - c_2 x^2 ...
		poly Q = berlekamp_massey(raw_seq);

		// 2. 构造分子 P
		// P = F * Q (保留前 m 项，m = Q.size() - 1)
		// F 是初始序列生成函数 F(x) = \sum a_i x^i
		// 我们只需要 Q 的度数这么多项即可计算 P
		int m = Q.size() - 1;
		// 如果 Q 只有常数项 1 (即序列全为0)，直接返回 0
		if (m == 0) return 0;

		poly A(raw_seq.begin(), raw_seq.begin() + min((int)raw_seq.size(), m));
		poly P = poly_mul(A, Q);
		if (P.size() > m) P.resize(m);

		// 3. Bostan-Mori
		return bostan_mori(P, Q, k);
	}

	// -----------------------------------------------------------
	// 二维多项式乘法
	// -----------------------------------------------------------

	typedef vector<vector<int>> poly2d;

	// 辅助函数：矩阵转置
	// 将 R x C 的矩阵转置为 C x R
	void matrix_transpose(poly2d &Mat, int R, int C) {
		poly2d T(C, poly(R));
		for (int i = 0; i < R; ++i) {
			for (int j = 0; j < C; ++j) {
				// 注意：NTT 过程中可能会 resize，所以要确保访问安全
				// 在本流程中，调用此函数时矩阵已经是满的 R*C 大小
				T[j][i] = Mat[i][j];
			}
		}
		Mat = move(T);
	}

	poly2d poly_mul_2d(const poly2d &A, const poly2d &B) {
		if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) return {};

		int nA = A.size(), mA = A[0].size();
		int nB = B.size(), mB = B[0].size();

		// 结果矩阵的实际大小
		int finalN = nA + nB - 1;
		int finalM = mA + mB - 1;

		// 计算 NTT 需要的 2 的幂次长度
		// 注意：NTT_init 会修改全局的 RR 数组，所以每次切换维度处理前都要重新调用
		int limN = NTT_init(finalN);
		int limM = NTT_init(finalM);

		// 初始化容器，大小预设为 limN (行数)
		poly2d FA(limN), FB(limN);

		// 复制数据
		for (int i = 0; i < nA; ++i) FA[i] = A[i];
		for (int i = 0; i < nB; ++i) FB[i] = B[i];
		// 剩下的行 (nA ~ limN-1) 默认为空 vector，会在接下来的 NTT 中被自动 resize 补 0

		// ==========================================
		// 步骤 1: 对每一行做 NTT (变换 y 轴方向)
		// ==========================================
		NTT_init(finalM); // 初始化 RR 数组用于长度 limM
		for (int i = 0; i < limN; ++i) {
			NTT(FA[i], 1, limM); // NTT 内部会自动 resize 到 limM
			NTT(FB[i], 1, limM);
		}

		// ==========================================
		// 步骤 2: 对每一列做 NTT (变换 x 轴方向)
		// ==========================================
		// 为了利用现有的 1D NTT 且优化缓存，我们进行转置
		// 转置后：FA 变为 limM 行, limN 列
		matrix_transpose(FA, limN, limM);
		matrix_transpose(FB, limN, limM);

		NTT_init(finalN); // 初始化 RR 数组用于长度 limN
		for (int i = 0; i < limM; ++i) {
			NTT(FA[i], 1, limN);
			NTT(FB[i], 1, limN);
		}

		// ==========================================
		// 步骤 3: 点值相乘
		// ==========================================
		// 此时矩阵大小为 limM * limN (因为转置过)
		for (int i = 0; i < limM; ++i) {
			for (int j = 0; j < limN; ++j) {
				FA[i][j] = 1ll * FA[i][j] * FB[i][j] % mod;
			}
		}

		// ==========================================
		// 步骤 4: 对每一列做 INTT (x 轴逆变换)
		// ==========================================
		// 当前维度仍是 limN，无需再次 NTT_init
		for (int i = 0; i < limM; ++i) {
			NTT(FA[i], 0, limN);
		}

		// ==========================================
		// 步骤 5: 转置回来
		// ==========================================
		// 变回 limN 行, limM 列
		matrix_transpose(FA, limM, limN);

		// ==========================================
		// 步骤 6: 对每一行做 INTT (y 轴逆变换)
		// ==========================================
		NTT_init(finalM); // 切换回 limM 的 RR 数组
		for (int i = 0; i < limN; ++i) {
			NTT(FA[i], 0, limM);
		}

		// ==========================================
		// 步骤 7: 截断到实际大小
		// ==========================================
		FA.resize(finalN);
		for (int i = 0; i < finalN; ++i) {
			FA[i].resize(finalM);
		}

		return FA;
	}

	// -----------------------------------------------------------
	// Power Projection
	// -----------------------------------------------------------
	// 求解 h_i = [x^n] f(x)^i g(x) for i=0...ny
	// 假设 [x^0]f(x) = 0
	// 复杂度: O(N log^2 N)
	poly power_projection(poly f, poly g, int n = -1, int ny = -1) {
		if (n == -1) n = f.size() - 1;
		if (ny == -1) ny = n;

		// P(x, y) = g(x) (y 的 0 次项)
		// Q(x, y) = 1 - y * f(x)

		// 初始状态:
		// P 的 x 次数为 n, y 次数为 1
		// Q 的 x 次数为 n, y 次数为 2

		vector<poly> P(n + 1, poly(1));
		vector<poly> Q(n + 1, poly(2));

		for (int i = 0; i <= n; ++i) {
			if (i < (int)g.size()) P[i][0] = g[i];
			else P[i][0] = 0;
		}

		// Q = 1 - y * f(x)
		// [x^0]Q = 1 (因为 [x^0]f=0，所以常数项只由 1 提供)
		// [x^i]Q = -f[i] * y
		Q[0][0] = 1; Q[0][1] = (f.size() > 0 ? minus(0, f[0]) : 0);
		for (int i = 1; i <= n; ++i) {
			Q[i][0] = 0;
			Q[i][1] = (i < (int)f.size()) ? minus(0, f[i]) : 0;
		}

		int cur_n = n; // 当前 x 的度数
		int cur_k = 1; // 当前 y 的度数 (2^step)

		while (cur_n > 0) {
			// 1. 构造 R(x, y) = Q(-x, y)
			// 也就是将 Q 中 x 的奇次项系数取反
			vector<poly> R = Q;
			for (int i = 1; i <= cur_n; i += 2) {
				for (auto &val : R[i]) val = minus(0, val);
			}

			// 2. 二元卷积
			// S = P * R
			// T = Q * R = Q(x)Q(-x) = V(x^2)
			vector<poly> S = poly_mul_2d(P, R);
			vector<poly> T = poly_mul_2d(Q, R);

			// 3. 提取系数，缩小 x 次数，倍增 y 次数
			// New P: 取 S 中 x 的奇偶项 (取决于 n 的奇偶性)
			// New Q: 取 T 中 x 的偶数项

			int next_n = cur_n / 2;
			int next_k = cur_k * 2;

			vector<poly> next_P(next_n + 1);
			vector<poly> next_Q(next_n + 1);

			// 目标 y 长度大约是 next_k + 1 (或者稍微多一点，因为卷积扩张了)
			// 我们只需要保留前 next_k + 1 项即可吗？
			// 实际上 y 的次数会翻倍，我们需要保留足够的 y 精度。
			// 理论上只需要 mod y^(n+1) 精度，但这里 n 指的是最初的 n。
			// 在这一步，我们保留 next_k * 2 左右的安全长度。

			int parity = cur_n % 2;
			for (int i = 0; i <= next_n; ++i) {
				// P' = [x^{2i+parity}] S
				if (2 * i + parity < (int)S.size()) {
					next_P[i] = S[2 * i + parity];
				} else {
					next_P[i] = {0};
				}

				// Q' = [x^{2i}] T
				if (2 * i < (int)T.size()) {
					next_Q[i] = T[2 * i];
				} else {
					next_Q[i] = {0};
				}

				// 裁剪 y 的大小以保证复杂度 (重要!)
				// 实际上下一轮需要的 y 精度是 next_k + 1
				// 卷积后 y 的大小变成了 ~2*cur_k，这就是 next_k
				if ((int)next_P[i].size() > next_k + 1) next_P[i].resize(next_k + 1);
				if ((int)next_Q[i].size() > next_k + 1) next_Q[i].resize(next_k + 1);
			}

			P = next_P;
			Q = next_Q;
			cur_n /= 2;
			cur_k *= 2;
		}

		// 最终 P[0] 和 Q[0] 都是关于 y 的多项式
		// 答案是 P[0](y) / Q[0](y) mod y^(ny+1)
		poly final_P = P[0];
		poly final_Q = Q[0];

		int target_deg = ny + 1;
		if ((int)final_P.size() > target_deg) final_P.resize(target_deg);
		if ((int)final_Q.size() > target_deg) final_Q.resize(target_deg);

		poly inv_Q = poly_inv(final_Q, target_deg);
		poly ans = poly_mul(final_P, inv_Q);
		ans.resize(target_deg);

		return ans;
	}


}

//bm-bm algo:
//1. compute array a with bruteforce
//2. call bm algo to obtain v
//3. reverse v
//4. then a[k] = x^k mod v \dot a

using Poly::poly;
using Poly::poly_arcsin;
using Poly::poly_arctan;

int n, m, x, k, type;
poly f, g;
char s[N];

int main()
{
    Poly::init(18);//2^21 = 2,097,152,根据题目数据多项式项数的大小自由调整，注意大小需要跟deer数组同步(21+1=22)

    read(n), read(type);

    for(int i = 0; i < n; ++ i)
        read(x), f.push_back(x);

    if(type == 0) g = poly_arcsin(f, n);
    else g = poly_arctan(f, n);

    for(int i = 0; i < n; ++ i)
        printf("%d ", g[i]);
    return 0;
}
