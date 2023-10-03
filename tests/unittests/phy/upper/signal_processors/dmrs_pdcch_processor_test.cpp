/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "../../support/resource_grid_mapper_test_doubles.h"
#include "dmrs_pdcch_processor_test_data.h"
#include "srsran/phy/support/support_factories.h"
#include "srsran/phy/upper/signal_processors/signal_processor_factories.h"

using namespace srsran;

int main()
{
  std::shared_ptr<pseudo_random_generator_factory> prg_factory = create_pseudo_random_generator_sw_factory();
  TESTASSERT(prg_factory);

  std::shared_ptr<dmrs_pdcch_processor_factory> dmrs_pdcch_factory =
      create_dmrs_pdcch_processor_factory_sw(prg_factory);
  TESTASSERT(dmrs_pdcch_factory);

  // Create DMRS-PDSCH processor.
  std::unique_ptr<dmrs_pdcch_processor> dmrs_pdcch = dmrs_pdcch_factory->create();

  for (const test_case_t& test_case : dmrs_pdcch_processor_test_data) {
    int prb_idx_high = test_case.config.rb_mask.find_highest();
    TESTASSERT(prb_idx_high > 1);
    unsigned max_prb   = static_cast<unsigned>(prb_idx_high + 1);
    unsigned max_symb  = test_case.config.start_symbol_index + test_case.config.duration;
    unsigned max_ports = test_case.config.precoding.get_nof_ports();

    // Prepare resource grid and resource grid mapper spies.
    resource_grid_writer_spy              grid(max_ports, max_symb, max_prb);
    std::unique_ptr<resource_grid_mapper> mapper = create_resource_grid_mapper(max_ports, NRE * max_prb, grid);

    // Map DMRS-PDCCH using the test case arguments.
    dmrs_pdcch->map(*mapper, test_case.config);

    // Load output golden data.
    const std::vector<resource_grid_writer_spy::expected_entry_t> testvector_symbols = test_case.symbols.read();

    // Assert resource grid entries.
    grid.assert_entries(testvector_symbols);
  }

  return 0;
}
