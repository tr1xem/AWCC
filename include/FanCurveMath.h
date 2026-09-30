#pragma once

struct FanCurvePoint {
    int tempC = 40;
    int extra = 0;
};

// Extra at tempC along four points. Below the first point and above the last
// point the curve holds that end. This is a boost on the thermal mode, not a
// percent of maximum fan speed.
inline int ExtraFor(const FanCurvePoint *points, int tempC) {
    FanCurvePoint sorted[4];
    for (int i = 0; i < 4; ++i)
        sorted[i] = points[i];
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            if (sorted[j].tempC < sorted[i].tempC) {
                const FanCurvePoint swap = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = swap;
            }
        }
    }
    auto hold = [](int extra) {
        if (extra < 0)
            return 0;
        if (extra > 100)
            return 100;
        return extra;
    };
    if (tempC <= sorted[0].tempC)
        return hold(sorted[0].extra);
    if (tempC >= sorted[3].tempC)
        return hold(sorted[3].extra);
    for (int i = 0; i < 3; ++i) {
        if (tempC > sorted[i + 1].tempC)
            continue;
        const int span = sorted[i + 1].tempC - sorted[i].tempC;
        if (span <= 0)
            return hold(sorted[i + 1].extra);
        const float t = static_cast<float>(tempC - sorted[i].tempC) /
                        static_cast<float>(span);
        const float extra =
            static_cast<float>(sorted[i].extra) +
            t * static_cast<float>(sorted[i + 1].extra - sorted[i].extra);
        return hold(static_cast<int>(extra + 0.5F));
    }
    return hold(sorted[3].extra);
}
