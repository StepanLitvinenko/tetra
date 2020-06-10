// Implementation of ViterbiCodec.
//
// Author: Min Xu <xukmin@gmail.com>
// Date: 01/30/2015

#include "viterbidecoder.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>
#include <cmath>


int ReverseBits(int num_bits, int input) {
    assert(input < (1 << num_bits));
    int output = 0;
    while (num_bits-- > 0) {
        output = (output << 1) + (input & 1);
        input >>= 1;
    }
    return output;
}

ViterbiCodec::ViterbiCodec(int constraint, const std::vector<int>& polynomials)
    : constraint_(constraint), polynomials_(polynomials) {
    assert(!polynomials_.empty());
    for (size_t i = 0; i < polynomials_.size(); i++) {
        assert(polynomials_[i] > 0);
        assert(polynomials_[i] < (1 << constraint_));
    }
    InitializeOutputs();}

int ViterbiCodec::num_parity_bits() const {
    return static_cast<int>(polynomials_.size());
}

int ViterbiCodec::NextState(int current_state, int input) const {
    return (current_state >> 1) | (input << (constraint_ - 2));
}
std::vector< uint16_t> ViterbiCodec::Output(int current_state, int input)  {

    return outputs_vector.at(current_state | (input << (constraint_ - 1)));

}

std::vector< uint16_t> ViterbiCodec::Encode(std::vector< uint16_t>& bits)  {
   std::vector< uint16_t>  encoded;
    int state = 0;


    // Encode the message bits.
    for (size_t i = 0; i < bits.size(); i++) {
        uint16_t input = bits[i];
        assert(input == 0 || input == 1);
        auto temp = Output(state, input);
        std::copy(temp.begin(), temp.end(), back_inserter(encoded));
        state = NextState(state, input);
    }

    // Encode (constaint_ - 1) flushing bits.
    for (int i = 0; i < constraint_ - 1; i++) {
         auto temp = Output(state, 0);
        std::copy(temp.begin(), temp.end(), back_inserter(encoded));
        state = NextState(state, 0);
    }

    return encoded;
}

void ViterbiCodec::InitializeOutputs() {

    outputs_vector.resize(pow(2,constraint_));
    for (auto &a:outputs_vector) a.resize(polynomials_.size());

    for (size_t i = 0; i < outputs_vector.size(); i++) {
        for (int j = 0; j < num_parity_bits(); j++) {
            // Reverse polynomial bits to make the convolution code simpler.
            int polynomial = ReverseBits(constraint_, polynomials_[j]);
            auto input = i;
            auto output = 0;
            for (int k = 0; k < constraint_; k++) {
                output ^= (input & 1) & (polynomial & 1);
                polynomial >>= 1;
                input >>= 1;
            }
            outputs_vector[i][j] = output;
        }
    }
}

float ViterbiCodec::BranchMetric(const std::vector <float>& bits,
                               int source_state,
                               int target_state)  {
    assert(bits.size() == num_parity_bits());
    assert((target_state & ((1 << (constraint_ - 2)) - 1)) == source_state >> 1);
    const std::vector <uint16_t> output = Output(source_state, target_state >> (constraint_ - 2));
    return EuclidDistance(bits, output);
   }

std::pair<float, int> ViterbiCodec::PathMetric(
        const std::vector <float>& bits,
        const std::vector<float>& prev_path_metrics,
        int state)  {
    int s = (state & ((1 << (constraint_ - 2)) - 1)) << 1;
    auto source_state1 = s | 0;
    auto source_state2 = s | 1;

    float pm1 = prev_path_metrics[source_state1];
    if (pm1 < std::numeric_limits<int>::max()) {
        pm1 += BranchMetric(bits, source_state1, state);
    }
    float pm2 = prev_path_metrics[source_state2];
    if (pm2 < std::numeric_limits<int>::max()) {
        pm2 += BranchMetric(bits, source_state2, state);
    }

    if (pm1 <= pm2) {
        return std::make_pair(pm1, source_state1);
    } else {
        return std::make_pair(pm2, source_state2);
    }
}

void ViterbiCodec::UpdatePathMetrics(const std::vector <float>& bits,
                                     std::vector<float>* path_metrics,
                                     Trellis* trellis)  {
    std::vector<float> new_path_metrics(path_metrics->size());
    std::vector<int> new_trellis_column(1 << (constraint_ - 1));
    for (size_t i = 0; i < path_metrics->size(); i++) {
        std::pair<float, int> p = PathMetric(bits, *path_metrics, static_cast<int>(i));
        new_path_metrics[i] = p.first;
        new_trellis_column[i] = p.second;
    }

    *path_metrics = new_path_metrics;
    trellis->push_back(new_trellis_column);
}

float ViterbiCodec::EuclidDistance(const std::vector<float> &x, const std::vector<uint16_t> &y)
{
        assert(x.size() == y.size());
        float distance = 0;
        float yFloat;
        for (size_t i = 0; i < x.size(); i++) {
            yFloat = y[i];
            distance += pow(yFloat - x[i],2);
        }
        return sqrt(distance);


}

std::vector <uint16_t> ViterbiCodec::Decode(const std::vector <float>& bits)  {
    int byteSize = 8;
    uint32_t bitPack = 0;
    uint32_t sizePack = sizeof(uint32_t)*byteSize;

    bitPack = bitPack^1<<(sizePack-1);
   // bitPack = bitPack^1;


    uint32_t bitPack2 = std::numeric_limits<int>::max();
    // Compute path metrics and generate trellis.
    Trellis trellis;
    std::vector<float> path_metrics(1 << (constraint_ - 1),
                                  std::numeric_limits<int>::max());
    path_metrics.front() = 0;
    for (int i = 0; i < static_cast<int>(bits.size()); i += num_parity_bits()) {

        std::vector <float> current_bits;
        if (i<=(bits.size()-num_parity_bits()))
           std::copy(bits.begin() + i, bits.begin() + i + num_parity_bits(), back_inserter(current_bits));
        else {
            current_bits.push_back(bits[i]);
            for (int i = 0; i < num_parity_bits() - 1; ++i) current_bits.push_back(0);
        }
        // If some bits are missing, fill with trailing zeros.
        // This is not ideal but it is the best we can do.
        UpdatePathMetrics(current_bits, &path_metrics, &trellis);
    }
    // Traceback.
    std::vector <uint16_t> decoded;
    int state = std::min_element(path_metrics.begin(), path_metrics.end()) -
            path_metrics.begin();
    for (int i( trellis.size() - 1),j(0) ; i >= 0; i--, ++j) {

        uint16_t temp = state >> (constraint_ - 2) ? 1 : 0;

        decoded.push_back(temp);

        bitPack = bitPack|(temp<<((sizePack-2) - j));





        state = trellis[i][state];
    }




    std::reverse(decoded.begin(), decoded.end());
//    for (int i = 0; i < constraint_ -1; ++i)
//        decoded.pop_back();
    // Remove (constraint_ - 1) flushing bits.
    return decoded;
}
