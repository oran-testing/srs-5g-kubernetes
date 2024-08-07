/*
 *
 * Copyright 2021-2024 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "srsran/phy/upper/signal_processors/ptrs/ptrs_pdsch_generator_factory.h"
#include "ptrs_pdsch_generator_impl.h"

using namespace srsran;

namespace {

/// PT-RS for PDSCH generator generic factory.
class ptrs_pdsch_generator_generic_factory : public ptrs_pdsch_generator_factory
{
public:
  /// Creates a PT-RS for PDSCH generator generic factory.
  ptrs_pdsch_generator_generic_factory(std::shared_ptr<pseudo_random_generator_factory> pseudo_random_gen_factory_) :
    pseudo_random_gen_factory(std::move(pseudo_random_gen_factory_))
  {
    srsran_assert(pseudo_random_gen_factory, "Invalid factory.");
  }

  // See interface for documentation.
  std::unique_ptr<ptrs_pdsch_generator> create() override
  {
    return std::make_unique<ptrs_pdsch_generator_generic_impl>(pseudo_random_gen_factory->create());
  }

private:
  /// Pseudo-random sequence generator factory.
  std::shared_ptr<pseudo_random_generator_factory> pseudo_random_gen_factory;
};

} // namespace

std::shared_ptr<ptrs_pdsch_generator_factory> srsran::create_ptrs_pdsch_generator_generic_factory(
    std::shared_ptr<pseudo_random_generator_factory> pseudo_random_gen_factory)
{
  return std::make_shared<ptrs_pdsch_generator_generic_factory>(std::move(pseudo_random_gen_factory));
}