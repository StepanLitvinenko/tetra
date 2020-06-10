// Viterbi Codec.
//
// Author: Min Xu <xukmin@gmail.com>
// Date: 01/30/2015

#ifndef VITERBI_H_
#define VITERBI_H_

#include <ostream>
#include <string>
#include <utility>
#include <vector>

// This class implements both a Viterbi Decoder and a Convolutional Encoder.
class ViterbiCodec {
public:
    // Note about Polynomial Descriptor of a Convolutional Encoder / Decoder.
    // A generator polymonial is built as follows: Build a binary number
    // representation by placing a 1 in each spot where a connection line from
    // the shift register feeds into the adder, and a zero elsewhere. There are 2
    // ways to arrange the bits:
    // 1. msb-current
    //    The MSB of the polynomial corresponds to the current input, while the
    //    LSB corresponds to the oldest input that still remains in the shift
    //    register.
    //    This representation is used by MATLAB. See
    //    http://radio.feld.cvut.cz/matlab/toolbox/comm/tutor124.html
    // 2. lsb-current
    //    The LSB of the polynomial corresponds to the current input, while the
    //    MSB corresponds to the oldest input that still remains in the shift
    //    register.
    //    This representation is used by the Spiral Viterbi Decoder Software
    //    Generator. See http://www.spiral.net/software/viterbi.html
    // We use 2.
    ViterbiCodec(int constraint, const std::vector<int>& polynomials);


    std::vector<uint16_t> Encode(std::vector<uint16_t> &bits) ;

    std::vector<uint16_t> Decode(const std::vector<float> &bits);

    int constraint() const { return constraint_; }

    const std::vector<int>& polynomials() const { return polynomials_; }

private:
    // Suppose
    //
    //     Trellis trellis;
    //
    // Then trellis[i][s] is the state in the (i - 1)th iteration which leads to
    // the current state s in the ith iteration.
    // It is used for traceback.

    std::vector<std::vector<uint16_t>> outputs_vector;


    typedef std::vector<std::vector<int> > Trellis;

    int num_parity_bits() const;

    void InitializeOutputs();

    int NextState(int current_state, int input) const;

    std::vector<uint16_t> Output(int current_state, int input);

    float BranchMetric(const std::vector<float> &bits,
                     int source_state,
                     int target_state);

    // Given num_parity_bits() received bits, compute and returns path
    // metric and its corresponding previous state.
    std::pair<float, int> PathMetric(const std::vector<float> &bits,
                                   const std::vector<float> &prev_path_metrics,
                                   int state);

    // Given num_parity_bits() received bits, update path metrics of all states
    // in the current iteration, and append new traceback vector to trellis.
    void UpdatePathMetrics(const std::vector<float> &bits,
                           std::vector<float> *path_metrics,
                           Trellis* trellis);

    const int constraint_;
    const std::vector<int> polynomials_;
    float EuclidDistance(const std::vector<float> &x, const std::vector <uint16_t>& y);
    // The output table.
    // The index is current input bit combined with previous inputs in the shift
    // register. The value is the output parity bits in string format for
    // convenience, e.g. "10". For example, suppose the shift register contains
    // 0b10 (= 2), and the current input is 0b1 (= 1), then the index is 0b110 (=
    // 6).

};


int ReverseBits(int num_bits, int input);

#endif  // VITERBI_H_
