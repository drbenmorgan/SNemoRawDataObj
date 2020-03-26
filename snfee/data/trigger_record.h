//! \file  snfee/data/trigger_record.h
//! \brief Description of the SuperNEMO trigger raw record
//
// Copyright (c) 2018-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of SNFrontEndElectronics.
//
// SNFrontEndElectronics is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SNFrontEndElectronics is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SNFrontEndElectronics. If not, see <http://www.gnu.org/licenses/>.

#ifndef SNFEE_DATA_TRIGGER_RECORD_H
#define SNFEE_DATA_TRIGGER_RECORD_H

// Standard Library:
#include <limits>
#include <memory>
#include <vector>

// Third Party Libraries:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>

// This project:
#include <snfee/data/raw_record_interface.h>
#include <snfee/data/time.h>
#include <snfee/data/utils.h>
#include <snfee/geometry.h>

namespace snfee {
  namespace data {

    /// \brief SuperNEMO trigger board data record
    class trigger_record : public datatools::i_tree_dumpable,
                           public datatools::i_serializable,
                           public raw_record_interface {
    public:
      static const int32_t INVALID_NUMBER = -1;
      static const uint32_t INVALID_CLOCKTICK =
        std::numeric_limits<uint32_t>::max();

      enum trigger_mode_type {
        TRIGGER_MODE_INVALID = 0,
        TRIGGER_MODE_CALO_ONLY = 1,
        TRIGGER_MODE_CALO_TRACKER_TIME_COINC = 2,
        TRIGGER_MODE_CARACO = 3,
        TRIGGER_MODE_OPEN_DELAYED = 4,
        TRIGGER_MODE_APE = 5,
        TRIGGER_MODE_DAVE = 6,
        TRIGGER_MODE_SCREENING = 7
      };
      static std::string trigger_mode_label(const trigger_mode_type);

      /// Check if the record is complete
      bool is_complete() const;

      /// Smart print
      ///
      /// Usage:
      /// \code
      /// snfee::data::trigger_record myTrigRec
      /// ...
      /// boost::property_tree::ptree poptions;
      /// poptions.put("title", "Trigger record:");
      /// poptions.put("indent", ">>> ");
      /// myTrigRec.print_tree(std::clog, poptions);
      /// \endcode
      virtual void print_tree(std::ostream& out_ = std::clog,
                              const boost::property_tree::ptree& options_ =
                                empty_options()) const override;

      /// Set the hit ID
      void set_hit_num(const int32_t);

      //! Return the hit number
      int32_t get_hit_num() const override;

      /// Set the trigger ID
      void set_trigger_id(const int32_t);

      /// Return the trigger ID
      int32_t get_trigger_id() const override;

      /// Return the L2 trigger mode
      trigger_mode_type get_trigger_mode() const;

      /// Check if the progenitor trigger ID is set
      bool has_progenitor_trigger_id() const;

      /// Set the progenitor trigger ID
      void set_progenitor_trigger_id(const int32_t);

      /// Return the progenitor_trigger ID
      int32_t get_progenitor_trigger_id() const;

      /// Set the 1600 ns clocktick at L2 trigger decision
      void set_l2_clocktick_1600ns(const uint32_t);

      /// Return the 1600 ns clocktick at L2 trigger decision
      uint32_t get_l2_clocktick_1600ns() const;

      //! Initialize the record with values
      void make(const int32_t trigger_hit_num_,
                const int32_t trigger_id_,
                const trigger_mode_type trigger_mode_);

      //! Reset the record
      void invalidate() override;

      //! Convert the trigger decision timestamp to a timestamp object
      timestamp convert_timestamp() const;

    private:
      int32_t _hit_num_ = INVALID_NUMBER;                      //!< Hit number
      int32_t _trigger_id_ = INVALID_TRIGGER_ID;               //!< Trigger ID
      trigger_mode_type _trigger_mode_ = TRIGGER_MODE_INVALID; //!< Trigger mode

      uint32_t _l2_clocktick_1600ns_ =
        INVALID_CLOCKTICK; //!< L2 trigger decision timestamp
      int32_t _progenitor_trigger_id_ =
        INVALID_TRIGGER_ID; //!< Progenitor trigger ID (for APE/DAVE trigger
                            //!< mode)

      // bool
      // _cell_matrix_[geometry::NSIDES][geometry::TRACKER_NLAYERS][geometry::TRACKER_NROWS];
      // uint32_t        _cell_matrix_clocktick_1600ns_ = 0;
      // bool            _l1_calo_decision_             = false;
      // uint32_t        _l1_calo_clocktick_25ns_       = 0;
      // bool            _l1_tracker_decision_          = false;
      // uint32_t        _l1_tracker_clocktick_1600ns_  = 0;

      DATATOOLS_SERIALIZATION_DECLARATION()
    };

    typedef std::shared_ptr<trigger_record> trigger_record_ptr;
    typedef std::shared_ptr<const trigger_record> const_trigger_record_ptr;

  } // namespace data
} // namespace snfee

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(snfee::data::trigger_record,
                        "snfee::data::trigger_record")

#endif // SNFEE_DATA_TRIGGER_RECORD_H
