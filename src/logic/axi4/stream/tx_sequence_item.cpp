/* Copyright 2018 Tymoteusz Blazejczyk
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "logic/axi4/stream/tx_sequence_item.hpp"
#include "logic/printer/json.hpp"

using logic::axi4::stream::tx_sequence_item;

namespace {
namespace field {

class idle : public uvm::uvm_object {
public:
    explicit idle(const logic::range& value) :
        m_range{value}
    { }
protected:
    void do_print(const uvm::uvm_printer& printer) const override {
        printer.print_field_int("min", m_range.min(), -1, uvm::UVM_DEC);
        printer.print_field_int("max", m_range.max(), -1, uvm::UVM_DEC);
    }

    logic::range m_range{};
};

class width_value : public uvm::uvm_object {
public:
    explicit width_value(const logic::bitstream& bits) :
        m_width{bits.size()}
    {
        for (std::size_t i = 0; i < m_width; ++i) {
            m_value[int(i)] = bool(bits[i]);
        }
    }
protected:
    void do_print(const uvm::uvm_printer& printer) const override {
        printer.print_field_int("width", m_width, -1, uvm::UVM_DEC);
        printer.print_field("value", m_value, int(m_width), uvm::UVM_HEX);
    }

    std::size_t m_width{};
    uvm::uvm_bitstream_t m_value{};
};

} /* namespace field */
} /* namespace */

static constexpr std::size_t TIMEOUT{10000};

tx_sequence_item::tx_sequence_item() :
    tx_sequence_item{"sequence_item"}
{ }

tx_sequence_item::tx_sequence_item(const std::string& name) :
    uvm::uvm_sequence_item{name},
    tid{},
    tdest{},
    timeout{TIMEOUT},
    idle{}
{ }

tx_sequence_item::~tx_sequence_item() = default;

std::string tx_sequence_item::convert2string() const {
    logic::printer::json json_printer;
    do_print(json_printer);
    return json_printer.emit();
}

void tx_sequence_item::do_print(const uvm::uvm_printer& printer) const {
    printer.print_field_int("timeout", timeout, -1, uvm::UVM_DEC);
    printer.print_object("idle", field::idle{idle});
    printer.print_object("tid", field::width_value{tid});
    printer.print_object("tdest", field::width_value{tdest});
}

void tx_sequence_item::do_pack(uvm::uvm_packer&) const { }

void tx_sequence_item::do_unpack(uvm::uvm_packer&) { }

void tx_sequence_item::do_copy(const uvm::uvm_object& rhs) {
    auto other = dynamic_cast<const tx_sequence_item*>(&rhs);
    if (other != nullptr) {
        this->idle = other->idle;
        this->timeout = other->timeout;
        this->tid = other->tid;
        this->tdest = other->tdest;
    }
    else {
        UVM_ERROR(get_name(), "Error in do_copy");
    }
}

bool tx_sequence_item::do_compare(const uvm::uvm_object& rhs,
        const uvm::uvm_comparer* /* comparer */) const {
    auto other = dynamic_cast<const tx_sequence_item*>(&rhs);
    auto status = false;

    if (other != nullptr) {
        status = (tid == other->tid) &&
            (tdest == other->tdest);
    }
    else {
        UVM_ERROR(get_name(), "Error in do_compare");
    }

    return status;
}
