/*
 *
 * Copyright 2021-2024 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "rrc_ue_srb_context.h"
#include "srsran/asn1/rrc_nr/ul_ccch_msg_ies.h"
#include "srsran/asn1/rrc_nr/ul_dcch_msg_ies.h"
#include "srsran/rrc/rrc_cell_context.h"
#include "srsran/rrc/rrc_ue.h"
#include "srsran/rrc/rrc_ue_config.h"

namespace srsran {
namespace srs_cu_cp {

/// RRC states (3GPP 38.331 v15.5.1 Sec 4.2.1)
enum class rrc_state { idle = 0, connected, connected_inactive };

/// Holds the RRC UE context used by the UE object and all its procedures.
class rrc_ue_context_t
{
public:
  rrc_ue_context_t(const ue_index_t                       ue_index_,
                   const rnti_t                           c_rnti_,
                   const rrc_cell_context&                cell_,
                   const rrc_ue_cfg_t&                    cfg_,
                   std::optional<rrc_ue_transfer_context> rrc_context_);

  const ue_index_t                                         ue_index; // UE index assigned by the DU processor
  const rnti_t                                             c_rnti;   // current C-RNTI
  const rrc_cell_context                                   cell;     // current cell
  const rrc_ue_cfg_t                                       cfg;
  rrc_state                                                state = rrc_state::idle;
  std::optional<rrc_meas_cfg>                              meas_cfg;
  std::optional<asn1::fixed_bitstring<39>>                 five_g_s_tmsi_part1;
  std::optional<cu_cp_five_g_s_tmsi>                       five_g_s_tmsi;
  uint64_t                                                 setup_ue_id;
  asn1::rrc_nr::establishment_cause_opts                   connection_cause;
  std::map<srb_id_t, ue_srb_context>                       srbs;
  std::optional<asn1::rrc_nr::ue_nr_cap_s>                 capabilities;
  std::optional<asn1::rrc_nr::ue_cap_rat_container_list_l> capabilities_list;
  std::optional<rrc_ue_transfer_context> transfer_context; // Context of old UE when created through mobility.
  bool                                   reestablishment_ongoing = false;
  srslog::basic_logger&                  logger;
};

} // namespace srs_cu_cp
} // namespace srsran
