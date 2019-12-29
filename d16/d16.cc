/**
 * https://topaz.github.io/paste/#XQAAAQDRBwAAAAAAAAARmknGRw8TogB3Oxzri2QTFyHNmsUnNCHOaRevztXy7bUO822AQlhnakR+/aGrvmTjyK0udup3dRYqrRjymhlAYGcyBrwk6eva45LLyK1Eno4JLNvVVspLH4IyKmVGIER8rcbcHNdbOu/ccnSnHO0yhpCQDJeASS0yTy5QVkewTxQOceypJUR2cXkT1ZQ/b2LPb8pAePKqj04vnBpr7B1PstrN9KZFBVvgRiscy5fv00PoeFF1Q/e+biWN9mXetDhfvNOU5LVcYdVi/SY8vYwYqwU9oriEXEkqn74y+mEsxvJJ6Tsyj/AgJ+JtlWZmtSPTJiw8Sbpku0MSTxY9ROFzMsO7yUOoRSgtuyOK9Z3FlfQW6itI2tP0Naex4gBpPrJWkHFlMGFenoFEt0bxTW+IgVgSDXrqu1AGsZFM4caccegBsyezyBjmx8fLye5sSsMa/lWzvOP4xl5objsAleuXJe3+L5YKCZu06Pg4knnQNugDqwffo8qGX3QEjk2AvDqo8dGe1z6px/KSblY0QkxEK1UlIJ3Tqah+pxp1xmFrjFJH9LzSmK4Nh2B9RCQZULS5vTNghoBinPERgJNR7G81i1FBCnQCh6TTy12m5jLyPwanqEbYJ6b+AWW3vq49S8pcMtgCZWFeDmm8LbKerbyiMGGMW2NdYn4RpFy2Khl4hZ22chOekJG4omaq4Wuyqbs1UJTpByXsNAFcbLNMGxObJ9Sx3PPNK82Kiguv4saJ6wC/8Vm1qMzthreHn+ZDVl7J8dP3vfJuUvSWaHeh4XyoC6jti0GgnDvYpy/vbN5ckPZc0JRHKler8iNKE/RSspBK1S85G5ijFn4fRGv3O+nZ3K03193xdf1ivVHVxzHournTOEf8y3wdW0RNHZXBqUNQi717H4plYajwVwy8tFKStqLldEUljHbcb/FPLeZLV+XambCJhadT4D0KuFelG/Lwc10rEuWN1X9tzB1BUzVN8V1eeySdZTHqPOrI904AYkn4RL7C4wk3nrtFp/fu65K/OAYeH/76OSyp
 * 
 * Can't figure out an acceptable solution for part2 after ten hours+, so get this great C++ solution.
 * The spirit of part2 is that we assume we have the message in the last half of the output(input),
 * and since the last half will always have positive one as pattern value, we can get output by iterating
 * through the input from behind (and forget about the input before the offset).
 */
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <array>
#include <algorithm>

using list_t = std::vector<int>;

std::pair<list_t, int> get_input()
{
    std::ifstream fin("./d16/input.txt");
    list_t rv;
    std::string ln;
    std::getline(fin, ln);
    int k = std::stoi(ln.substr(0, 7));
    rv.resize(ln.size());
    std::transform(ln.begin(), ln.end(), rv.begin(), [](auto c) { return c - '0'; });
    return { rv, k };
}

int pat_val(int indigit, int outdigit)
{
    constexpr std::array<int, 4> base_pattern{ 0, 1, 0, -1 };
    ++outdigit;
    ++indigit;
     return base_pattern[(indigit / outdigit) % 4];
}

list_t phase1(list_t in)
{
    list_t rv(in.size());

    for (auto od = 0; od < in.size(); ++od)
    {
        auto outd = 0;
        for (auto id = 0; id < in.size(); ++id)
            outd += in[id] * pat_val(id, od);
        rv[od] = std::abs(outd) % 10;
    }
    return rv;
}

void pt1(list_t vi)
{
    for (int n = 0; n < 100; ++n)
        vi = phase1(vi);

    std::cout << "pt1 = ";
    std::copy_n(vi.begin(), 8, std::ostream_iterator<int>(std::cout, ""));
    std::cout << '\n';
}

list_t repeat_n(list_t const& vi, int n)
{
    list_t vo(vi.size() * n);
    auto vob = vo.begin();
    for (int cnt = 0; cnt < n; ++cnt)
        vob = std::copy_n(vi.begin(), vi.size(), vob);
    return vo;
}

list_t extract(list_t const& li, int off)
{
    list_t rv(li.size() - off);
    std::copy(li.begin() + off, li.end(), rv.begin());
    return rv;
}

void phase2(list_t& vi)
{
    auto tot = 0;
    for (auto ib = vi.rbegin(); ib != vi.rend(); ++ib)
    {
        tot += *ib;
        *ib = std::abs(tot) % 10;
    }
}

void pt2(list_t vi, int off)
{
    auto vb = repeat_n(vi, 10000);
    auto vb2 = extract(vb, off);
    for (int n = 0; n < 100; ++n)
        phase2(vb2);

    std::cout << "pt2 = ";
    std::copy_n(vb2.begin(), 8, std::ostream_iterator<int>(std::cout, ""));
    std::cout << '\n';
}

int main()
{
    auto [vi, k] = get_input();

    pt1(vi);
    pt2(vi, k);
}
