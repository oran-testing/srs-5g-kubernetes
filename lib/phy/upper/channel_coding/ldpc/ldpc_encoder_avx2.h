/*
 *
 * Copyright 2021-2024 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

/// \file
/// \brief LDPC encoder - Declarations for AVX2-optimized version.

#pragma once

#include "ldpc_encoder_impl.h"

namespace srsran {

/// \brief LDPC encoder implementation based on AVX2 intrinsics.
/// \warning Instantiating an object of this class entails a heap memory allocation.
class ldpc_encoder_avx2 : public ldpc_encoder_impl
{
private:
  void select_strategy() override;
  void load_input(const bit_buffer& in) override;
  void preprocess_systematic_bits() override { (this->*systematic_bits)(); }
  void encode_high_rate() override { (this->*high_rate)(); }
  void encode_ext_region() override {}
  void write_codeblock(span<uint8_t> out, unsigned offset) const override;

  /// Alias for pointer to private methods.
  using strategy_method = void (ldpc_encoder_avx2::*)();

  /// Placeholder for strategy implementation of preprocess_systematic_bits.
  strategy_method systematic_bits;
  /// Placeholder for strategy implementation of encode_high_rate.
  strategy_method high_rate;

  /// Helper method to set the high-rate encoding strategy.
  template <unsigned NODE_SIZE_AVX2_PH>
  static strategy_method
  select_hr_strategy(ldpc_base_graph_type current_bg, uint8_t current_ls_index, unsigned node_size_avx2);

  /// Helper method to set the strategy for the systematic bits.
  template <unsigned NODE_SIZE_AVX2_PH>
  static strategy_method select_sys_bits_strategy(ldpc_base_graph_type current_bg, unsigned node_size_avx2);

  /// \brief Long lifting size version of preprocess_systematic_bits - short lifting size.
  /// \tparam BG_K_PH            Placeholder for the number of information nodes (i.e., K) for the current base graph.
  /// \tparam BG_M_PH            Placeholder for the number of check nodes (i.e., M) for the current base graph.
  /// \tparam NODE_SIZE_AVX2_PH  Placeholder for the number of AVX2 registers used to represent a code node.
  template <unsigned BG_K_PH, unsigned BG_M_PH, unsigned NODE_SIZE_AVX2_PH>
  void systematic_bits_inner();
  /// \brief Carries out the high-rate region encoding for BG1 and lifting size index 6 - long lifting size.
  /// \tparam NODE_SIZE_AVX2_PH  Placeholder for the number of AVX2 registers used to represent a code node.
  template <unsigned NODE_SIZE_AVX2_PH>
  void high_rate_bg1_i6_inner();
  /// \brief Carries out the high-rate region encoding for BG1 and lifting size index in {0, 1, 2, 3, 4, 5, 7} - long
  /// lifting size.
  /// \tparam NODE_SIZE_AVX2_PH  Placeholder for the number of AVX2 registers used to represent a code node.
  template <unsigned NODE_SIZE_AVX2_PH>
  void high_rate_bg1_other_inner();
  /// \brief Carries out the high-rate region encoding for BG2 and lifting size index in {3, 7} - long lifting size.
  /// \tparam NODE_SIZE_AVX2_PH  Placeholder for the number of AVX2 registers used to represent a code node.
  template <unsigned NODE_SIZE_AVX2_PH>
  void high_rate_bg2_i3_7_inner();
  /// \brief Carries out the high-rate region encoding for BG2 and lifting size index in {0, 1, 2, 4, 5, 6} - long
  /// lifting size.
  /// \tparam NODE_SIZE_AVX2_PH  Placeholder for the number of AVX2 registers used to represent a code node.
  template <unsigned NODE_SIZE_AVX2_PH>
  void high_rate_bg2_other_inner();
  /// Carries out the extended region encoding when the lifting size is long.
  void ext_region_inner(span<uint8_t> output_node, unsigned i_layer) const;

  /// Buffer containing the codeblock.
  std::array<uint8_t, ldpc::MAX_BG_N_FULL * ldpc::MAX_LIFTING_SIZE> codeblock_buffer;
  /// Length of the portion of the codeblock buffer actually used for the current configuration (as a number of AVX2
  /// registers).
  size_t codeblock_used_size = 0;

  /// Auxiliary buffer for storing chunks of codeblocks (corresponds to the four redundancy nodes of the high-rate
  /// region).
  std::array<uint8_t, bg_hr_parity_nodes * ldpc::MAX_LIFTING_SIZE> auxiliary_buffer;
  /// Length of the extended region actually used for the current configuration (as a number of AVX2 registers).
  size_t length_extended = 0;

  /// Auxiliary buffer for storing a single node during rotations.
  std::array<uint8_t, ldpc::MAX_LIFTING_SIZE> rotated_node_buffer;

  /// Node size as a number of AVX2 vectors.
  unsigned node_size_avx2 = 0;
};

} // namespace srsran
