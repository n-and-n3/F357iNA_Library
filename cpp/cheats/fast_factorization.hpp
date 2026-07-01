#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

namespace fast_factorization {

using ll = long long;
using u64 = std::uint64_t;
using u128 = __uint128_t;

namespace internal {

// Numbers <= SMALL_LIMIT are factored by a precomputed SPF table.
// For composite numbers <= SMALL_LIMIT^2, trial division by these primes is also used.
static constexpr int SMALL_LIMIT = 1'000'000;

struct SmallSieve {
    std::vector<int> spf;
    std::vector<int> primes;

    SmallSieve() : spf(SMALL_LIMIT + 1) {
        spf[0] = 0;
        spf[1] = 1;
        for (int i = 2; i <= SMALL_LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                long long v = 1LL * p * i;
                if (v > SMALL_LIMIT || p > spf[i]) break;
                spf[(int)v] = p;
            }
        }
    }
};

inline const SmallSieve& sieve() {
    static const SmallSieve s;
    return s;
}

inline u64 splitmix64(u64 x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

inline u64& rng_state() {
    static u64 state =
        splitmix64((u64)std::chrono::steady_clock::now().time_since_epoch().count() ^
                   0x243f6a8885a308d3ULL);
    return state;
}

inline void set_seed(u64 seed) {
    rng_state() = splitmix64(seed);
}

inline u64 rng_u64() {
    u64& s = rng_state();
    s += 0x9e3779b97f4a7c15ULL;
    return splitmix64(s);
}

inline u64 rng_mod(u64 mod) {
    // mod must be positive. Modulo bias is irrelevant for Pollard Rho constants.
    return rng_u64() % mod;
}

// Dynamic Montgomery multiplication for odd modulus mod < 2^63.
// This is the range needed for signed long long positive inputs.
struct Montgomery64 {
    u64 mod;
    u64 neg_inv; // -mod^{-1} modulo 2^64
    u64 r2;      // 2^128 modulo mod

    explicit Montgomery64(u64 m) : mod(m), neg_inv(1), r2(0) {
        assert((mod & 1) == 1);
        assert(mod < (1ULL << 63));

        // Newton iteration: inv = mod^{-1} modulo 2^64, then negate it.
        u64 inv = mod;
        for (int i = 0; i < 6; ++i) inv *= 2 - mod * inv;
        neg_inv = ~inv + 1;

        r2 = (u64)((u128(-1) % mod + 1) % mod); // 2^128 mod mod
    }

    u64 reduce(u128 x) const {
        u64 q = (u64)x * neg_inv;
        u64 t = (u64)((x + (u128)q * mod) >> 64);
        if (t >= mod) t -= mod;
        return t;
    }

    u64 init(u64 x) const {
        return reduce((u128)(x % mod) * r2);
    }

    u64 get(u64 x) const {
        return reduce(x);
    }

    u64 add(u64 a, u64 b) const {
        // Overflow-safe modular addition for a, b < mod.
        return (a >= mod - b) ? a - (mod - b) : a + b;
    }

    u64 sub(u64 a, u64 b) const {
        return (a >= b) ? a - b : mod - (b - a);
    }

    u64 mul(u64 a, u64 b) const {
        return reduce((u128)a * b);
    }

    u64 pow(u64 a_mont, u64 e) const {
        u64 res = init(1);
        while (e) {
            if (e & 1) res = mul(res, a_mont);
            a_mont = mul(a_mont, a_mont);
            e >>= 1;
        }
        return res;
    }
};

inline bool is_prime_small_u64(u64 n) {
    if (n <= 1) return false;
    const auto& sv = sieve();
    return sv.spf[(int)n] == (int)n;
}

inline bool miller_rabin_u64(u64 n) {
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    if (n < 4) return true;

    // Avoid Montgomery assertion for values outside signed long long range.
    // Public API passes n <= LLONG_MAX, so this branch is mainly defensive.
    assert(n < (1ULL << 63));

    u64 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    Montgomery64 mt(n);
    const u64 one = mt.init(1);
    const u64 minus_one = mt.init(n - 1);

    // Deterministic bases for all 64-bit integers. For signed long long inputs
    // this is more than enough, and still only 7 rounds.
    static constexpr std::array<u64, 7> bases = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (u64 a : bases) {
        if (a % n == 0) continue;
        u64 x = mt.pow(mt.init(a), d);
        if (x == one || x == minus_one) continue;
        bool probably_prime_for_this_base = false;
        for (int r = 1; r < s; ++r) {
            x = mt.mul(x, x);
            if (x == minus_one) {
                probably_prime_for_this_base = true;
                break;
            }
        }
        if (!probably_prime_for_this_base) return false;
    }
    return true;
}

inline bool fast_isprime_u64(u64 n) {
    if (n <= SMALL_LIMIT) return is_prime_small_u64(n);

    // Cheap filters. This removes many composites before Miller-Rabin.
    static constexpr std::array<int, 25> small_primes = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };
    for (int p : small_primes) {
        if (n == (u64)p) return true;
        if (n % (u64)p == 0) return false;
    }
    return miller_rabin_u64(n);
}

inline u64 pollard_rho(u64 n) {
    if ((n & 1) == 0) return 2;
    if (n % 3 == 0) return 3;
    if (fast_isprime_u64(n)) return n;

    assert(n < (1ULL << 63));
    Montgomery64 mt(n);
    const u64 one = mt.init(1);

    while (true) {
        const u64 c = mt.init(rng_mod(n - 1) + 1); // 1..n-1
        u64 y = mt.init(rng_mod(n - 2) + 2);       // 2..n-1
        u64 x = 0;
        u64 ys = 0;
        u64 q = one;
        u64 g = 1;
        u64 r = 1;
        static constexpr u64 M = 128;

        auto f = [&](u64 v) -> u64 {
            return mt.add(mt.mul(v, v), c);
        };

        while (g == 1) {
            x = y;
            for (u64 i = 0; i < r; ++i) y = f(y);

            for (u64 k = 0; k < r && g == 1; k += M) {
                ys = y;
                const u64 lim = std::min<u64>(M, r - k);
                for (u64 i = 0; i < lim; ++i) {
                    y = f(y);
                    q = mt.mul(q, mt.sub(x, y));
                }
                g = std::gcd(mt.get(q), n);
            }

            if (r > (1ULL << 61)) break; // retry with other constants
            r <<= 1;
        }

        if (g == 1) continue;

        if (g == n) {
            do {
                ys = f(ys);
                g = std::gcd(mt.get(mt.sub(x, ys)), n);
            } while (g == 1);
        }
        if (g != n) return g;
    }
}

inline void factor_small_u64(u64 n, std::vector<u64>& out) {
    const auto& sv = sieve();
    while (n > 1) {
        int p = sv.spf[(int)n];
        out.push_back((u64)p);
        n /= (u64)p;
    }
}

inline void factor_trial_u64(u64 n, std::vector<u64>& out) {
    const auto& sv = sieve();
    for (int p : sv.primes) {
        u64 pp = (u64)p;
        if (pp * pp > n) break;
        while (n % pp == 0) {
            out.push_back(pp);
            n /= pp;
        }
    }
    if (n > 1) out.push_back(n);
}

inline void factor_rec(u64 n, std::vector<u64>& out) {
    if (n <= 1) return;

    if (n <= SMALL_LIMIT) {
        factor_small_u64(n, out);
        return;
    }

    // Strip very small factors before Miller-Rabin / Pollard Rho.
    static constexpr std::array<int, 25> small_primes = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };
    for (int p : small_primes) {
        if (n % (u64)p == 0) {
            out.push_back((u64)p);
            factor_rec(n / (u64)p, out);
            return;
        }
    }

    if (fast_isprime_u64(n)) {
        out.push_back(n);
        return;
    }

    // If sqrt(n) <= 1e6, trial division by the precomputed primes is usually
    // faster and more stable than invoking Pollard Rho.
    if (n <= (u64)SMALL_LIMIT * (u64)SMALL_LIMIT) {
        factor_trial_u64(n, out);
        return;
    }

    u64 d = pollard_rho(n);
    factor_rec(d, out);
    factor_rec(n / d, out);
}

inline std::vector<std::pair<ll, ll>> compress_factors(std::vector<u64> factors) {
    std::sort(factors.begin(), factors.end());
    std::vector<std::pair<ll, ll>> result;
    for (u64 p : factors) {
        if (result.empty() || result.back().first != (ll)p) {
            result.emplace_back((ll)p, 1LL);
        } else {
            ++result.back().second;
        }
    }
    return result;
}

} // namespace internal

// Optional: set Pollard Rho's RNG seed for reproducible behavior.
inline void set_seed(u64 seed) {
    internal::set_seed(seed);
}

// Deterministic Miller-Rabin primality test for 0 <= n <= LLONG_MAX.
inline bool fast_isprime(ll n) {
    if (n < 2) return false;
    return internal::fast_isprime_u64((u64)n);
}

// Returns {{prime, exponent}, ...}, sorted by prime in ascending order.
// Defined for positive n. For n <= 1, returns an empty vector.
inline std::vector<std::pair<ll, ll>> fast_factorize(ll n) {
    if (n <= 1) return {};
    std::vector<u64> factors;
    internal::factor_rec((u64)n, factors);
    return internal::compress_factors(std::move(factors));
}

// Enumerates all positive divisors from a prime factorization represented as
// {{prime, exponent}, ...}. The returned vector is sorted in ascending order.
inline std::vector<ll> fast_divisor(const std::vector<std::pair<ll, ll>>& factorization) {
    std::vector<ll> divisors;
    divisors.push_back(1);

    std::size_t reserve_size = 1;
    for (auto [p, e] : factorization) {
        if (e < 0) continue;
        if (reserve_size <= std::numeric_limits<std::size_t>::max() / (std::size_t)(e + 1)) {
            reserve_size *= (std::size_t)(e + 1);
        }
    }
    divisors.reserve(reserve_size);

    for (auto [p, e] : factorization) {
        if (e <= 0) continue;
        const std::size_t base_size = divisors.size();
        ll mul = 1;
        for (ll k = 1; k <= e; ++k) {
            mul *= p;
            for (std::size_t i = 0; i < base_size; ++i) {
                divisors.push_back(divisors[i] * mul);
            }
        }
    }

    std::sort(divisors.begin(), divisors.end());
    return divisors;
}

// Factorizes n and enumerates all positive divisors. For n == 0, returns {}.
inline std::vector<ll> fast_divisor(ll n) {
    if (n == 0) return {};
    if (n < 0) return {}; // This library is intended for positive integers.
    return fast_divisor(fast_factorize(n));
}

} // namespace fast_factorization

