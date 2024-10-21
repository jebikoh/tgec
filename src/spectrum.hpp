#pragma once
#include <jtxlib/math/math.hpp>
#include <jtxlib/util/taggedptr.hpp>
#include <jtxlib/std/memory_resource.hpp>
#include <jtxlib/std/std.hpp>

namespace jtx {
#pragma region SampledSpectrum
    static constexpr int N_SPECTRUM_SAMPLES = 4;

    class SampledSpectrum {
    public:
        JTX_HOSTDEV
        explicit SampledSpectrum(const float c) { ASSERT(!jtx::isNaN(c)); data.fill(c); }

        JTX_HOSTDEV
        explicit SampledSpectrum(const jtx::span<const float> v) {
            ASSERT(v.size() == N_SPECTRUM_SAMPLES);
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                data[i] = v[i];
            }
        }

        JTX_HOSTDEV
        float operator[](const int i) const { return data[i]; }

        JTX_HOSTDEV
        float &operator[](const int i) { return data[i]; }

        JTX_HOSTDEV
        explicit operator bool() {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) if (data[i] != 0) return true;
            return false;
        }

        JTX_HOSTDEV
        SampledSpectrum &operator+=(const SampledSpectrum &other) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) data[i] += other[i];
            return *this;
        }

        JTX_HOSTDEV
        SampledSpectrum &operator+=(const float c) {
            ASSERT(!jtx::isNaN(c));
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) data[i] += c;
            return *this;
        }

        JTX_HOSTDEV
        SampledSpectrum &operator-=(const SampledSpectrum &other) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) data[i] -= other[i];
            return *this;
        }

        JTX_HOSTDEV
        SampledSpectrum &operator-=(const float c) {
            ASSERT(!jtx::isNaN(c));
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) data[i] -= c;
            return *this;
        }

        JTX_HOSTDEV
        SampledSpectrum &operator*=(const SampledSpectrum &other) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) data[i] *= other[i];
            return *this;
        }

        JTX_HOSTDEV
        SampledSpectrum &operator*=(const float c) {
            ASSERT(!jtx::isNaN(c));
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) data[i] *= c;
            return *this;
        }

        JTX_HOSTDEV
        SampledSpectrum &operator/=(const SampledSpectrum &other) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                ASSERT(other[i] != 0);
                data[i] /= other[i];
            }
            return *this;
        }

        JTX_HOSTDEV
        SampledSpectrum &operator/=(const float c) {
            ASSERT(c != 0);
            ASSERT(!jtx::isNaN(c));
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) data[i] /= c;
            return *this;
        }

        JTX_HOSTDEV
        SampledSpectrum operator+(const SampledSpectrum &other) const {
            SampledSpectrum result = *this;
            return result += other;
        }

        JTX_HOSTDEV
        SampledSpectrum operator+(const float c) const {
            SampledSpectrum result = *this;
            return result += c;
        }

        JTX_HOSTDEV
        friend SampledSpectrum operator+(const float c, const SampledSpectrum &s) {
            return s + c;
        }

        JTX_HOSTDEV
        SampledSpectrum operator-(const SampledSpectrum &other) const {
            SampledSpectrum result = *this;
            return result -= c;
        }

        JTX_HOSTDEV
        SampledSpectrum operator-(const float c) const {
            SampledSpectrum result = *this;
            return result -= c;
        }

        JTX_HOSTDEV
        friend SampledSpectrum operator-(const float c, const SampledSpectrum &s) {
            return s - c;
        }

        JTX_HOSTDEV
        SampledSpectrum operator*(const SampledSpectrum &other) const {
            SampledSpectrum result = *this;
            return result *= other;
        }

        JTX_HOSTDEV
        SampledSpectrum operator*(const float c) const {
            SampledSpectrum result = *this;
            return result *= c;
        }

        JTX_HOSTDEV
        friend SampledSpectrum operator*(const float c, const SampledSpectrum &s) {
            return s * c;
        }

        JTX_HOSTDEV
        SampledSpectrum operator/(const SampledSpectrum &other) const {
            SampledSpectrum result = *this;
            return result /= other;
        }

        JTX_HOSTDEV
        SampledSpectrum operator/(const float c) const {
            SampledSpectrum result = *this;
            return result /= c;
        }

        JTX_HOSTDEV
        bool operator==(const SampledSpectrum &other) const { return data == other.data; }

        JTX_HOSTDEV
        bool operator!=(const SampledSpectrum &other) const { return data != other.data; }

        JTX_HOSTDEV
        float minValue() const {
            float result = data[0];
            for (int i = 1; i < N_SPECTRUM_SAMPLES; ++i) result = jtx::min(result, data[i]);
            return result;
        }

        JTX_HOSTDEV
        float maxValue() const {
            float result = data[0];
            for (int i = 1; i < N_SPECTRUM_SAMPLES; ++i) result = jtx::max(result, data[i]);
            return result;
        }

        JTX_HOSTDEV
        float average() const {
            float result = 0.0f;
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) result += data[i];
            return result / N_SPECTRUM_SAMPLES;
        }

    private:
        array<float, N_SPECTRUM_SAMPLES> data;
    };

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum safeDiv(const SampledSpectrum &a, const SampledSpectrum &b) {
        SampledSpectrum result;
        for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
            if (b[i] != 0) result[i] = a[i] / b[i];
            else result[i] = 0.0f;
        }
        return result;
    }

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum lerp(const SampledSpectrum &a, const SampledSpectrum &b, float t) {
        return (1 - t) * a + b * t;
    }

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum sqrt(const SampledSpectrum &s) {
        SampledSpectrum result;
        for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) result[i] = jtx::sqrt(s[i]);
        return result;
    }

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum safeSqrt(const SampledSpectrum &s) {
        SampledSpectrum result;
        for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) result[i] = jtx::safeSqrt(s[i]);
        return result;
    }

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum pow(const SampledSpectrum &s, const float n) {
        SampledSpectrum result;
        for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) result[i] = jtx::pow(s[i], n);
        return result;
    }

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum exp(const SampledSpectrum &s) {
        SampledSpectrum result;
        for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) result[i] = jtx::exp(s[i]);
        return result;
    }

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum fastExp(const SampledSpectrum &s) {
        SampledSpectrum result;
        for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) result[i] = jtx::fastExp(s[i]);
        return result;
    }

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum clamp(const SampledSpectrum &s, const float lo, const float hi) {
        SampledSpectrum result;
        for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) result[i] = jtx::clamp(s[i], lo, hi);
        return result;
    }

    JTX_HOSTDEV JTX_INLINE
    SampledSpectrum clampZero(const SampledSpectrum &s) {
        SampledSpectrum result;
        for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) result[i] = jtx::clampZero(s[i]);
    }

    JTX_HOSTDEV JTX_INLINE
    float minValue(const SampledSpectrum &s) { return s.minValue(); }

    JTX_HOSTDEV JTX_INLINE
    float maxValue(const SampledSpectrum &s) { return s.maxValue(); }

    JTX_HOSTDEV JTX_INLINE
    float average(const SampledSpectrum &s) { return s.average(); }
#pragma endregion SampledSpectrum

    class SampledWavelengths;

    class ConstantSpectrum;

    class Spectrum : public jtx::TaggedPtr<ConstantSpectrum> {
    public:
        using TaggedPtr::TaggedPtr;

        JTX_HOSTDEV
        float operator()(float lambda) const {
            auto op = [&](auto ptr) { return (*ptr)(lambda); };
            return dispatch(op);
        }

        [[nodiscard]]
        JTX_HOSTDEV
        float maxValue() const {
            auto op = [&](auto ptr) { return ptr->maxValue(); };
            return dispatch(op);
        }

        JTX_HOSTDEV
        SampledSpectrum sample(const SampledWavelengths &lambda) const {
            auto op = [&](auto ptr) { return ptr->sample(lambda); };
            return dispatch(op);
        }

        [[nodiscard]]
        JTX_HOST
        std::string toString() const {
            if (getPtr() == nullptr) return "(nullptr)";

            auto op = [&](auto ptr) { return ptr->toString(); };
            return dispatch(op);
        }
    };

    class ConstantSpectrum {
    public:
        JTX_HOSTDEV
        explicit ConstantSpectrum(const float c) : c(c) {}

        JTX_HOSTDEV
        float operator()(float lambda) const { return c; }

        [[nodiscard]]
        JTX_HOSTDEV
        float maxValue() const { return c; }
    private:
        float c;
    };

    class DenselySampledSpectrum {};

    class PiecewiseLinearSpectrum {};

    class BlackbodySpectrum {};
}