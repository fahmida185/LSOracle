#include <ot/timer/timer.hpp>

namespace ot {

// Function: verilog
Timer& Timer::verilog(std::filesystem::path path) {
  
  // Create a verilog module
  auto module = std::make_shared<vlog::Module>();

  std::scoped_lock lock(_mutex);

  // reader
  auto reader = _taskflow.silent_emplace([this, module, path=std::move(path)] () {
    OT_LOGI("loading netlist ", path, " ...");
    *module = vlog::read_verilog(path);
  });

  // modifier
  auto modifier = _taskflow.silent_emplace([this, module] () mutable {
    OT_LOGI("add ", module->info());
    _verilog(*module);
  });

  // Build the task dependency
  reader.precede(modifier);

  // Build the dependency with the precedent
  _add_to_lineage(modifier);
    
  return *this;
}

// Procedure: _verilog
// Update the circuit from a given verilog module.
void Timer::_verilog(vlog::Module& module) {

  // Step 1: Scan the primary input and create a pin for each primary input port.
  // Each primary input is automatically connected to a net whose name is the same
  // as the name of the primary input.
  for(const auto& pi : module.inputs) {
    _insert_primary_input(pi);
  }
  
  // Step 2: Scan the primary output and create a pin for each primary output port.
  // Each primary output is automatically connected to a net whose name is the same
  // as the name of the primary output.
  for(const auto& po : module.outputs) {
    _insert_primary_output(po);
  }
  
  // Step 3: Scan the wires and insert a net for each wire. Each wire is automatically
  // attached to a net. The connection of each net is specified by the gates.
  for(const auto& wire : module.wires) {
    _insert_net(wire);
  }
  
  // Step 4: Scan the gate through verilog file. Insert a new gate for each gate
  // being iterated and a set of pins corresponding to each cellpin of the gate. Then,
  // for each pin-net mapping specified in the gate, connect the pin to the net.
  for(const auto& gate : module.gates) {
    _insert_gate(gate.name, gate.cell);
    for(const auto& [c, n] : gate.cellpin2net) {
      auto& pin = _insert_pin(gate.name + ':' + c);
      auto& net = _insert_net(n); 
      _connect_pin(pin, net);
    }
  }
}

};  // end of namespace ot. -----------------------------------------------------------------------
