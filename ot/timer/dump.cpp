#include <ot/timer/timer.hpp>

namespace ot {

// Function: dump_graph
std::string Timer::dump_graph() const {
  std::shared_lock lock(_mutex);
  return _dump_graph(); 
}

// Function: _dump_graph
std::string Timer::_dump_graph() const {
  
  std::ostringstream os;

  os << "digraph TimingGraph {\n";
  for(const auto& pin : _pins) {
    os << "  \"" << pin.second._name << "\";\n";
  }

  for(const auto& arc : _arcs) {
    os << "  \"" << arc._from._name << "\" -> \"" << arc._to._name << "\";\n";  
  }
  os << "}\n";

  return os.str();
}

// Function: dump_lineage
std::string Timer::dump_lineage() const {
  std::shared_lock lock(_mutex);
  return _dump_lineage();
}

// Function: _dump_lineage
std::string Timer::_dump_lineage() const {
  return _taskflow.dump();
}

// Function: dump_timer
std::string Timer::dump_timer() const {
  std::shared_lock lock(_mutex);
  return _dump_timer();
}

// Function: _dump_timer
std::string Timer::_dump_timer() const {
  
  std::ostringstream oss;

  oss << "OpenTimer " << OT_VERSION << '\n';
  
  // units
  if(_time_unit) {
    oss << "Time unit        : " << *_time_unit << '\n';
  }
  
  if(_capacitance_unit) {
    oss << "Capacitance unit : " << *_capacitance_unit << '\n';
  }

  if(_voltage_unit) {
    oss << "Voltage unit     : " << *_voltage_unit << '\n';
  }

  if(_resistance_unit) {
    oss << "Resistance unit  : " << *_resistance_unit << '\n';
  }

  if(_current_unit) {
    oss << "Current unit     : " << *_current_unit << '\n';
  }

  if(_power_unit) {
    oss << "Power unit       : "  << *_power_unit << '\n';
  }

  size_t num_cells = 0;

  FOR_EACH_EL_IF(el, _celllib[el]) {
    num_cells = std::max(num_cells, _celllib[el]->cells.size());
  }

  // design statistics
  oss << "# Pins           : " << _pins.size()  << '\n'
      << "# POs            : " << _pos.size()   << '\n'
      << "# PIs            : " << _pis.size()   << '\n'
      << "# Gates          : " << _gates.size() << '\n'
      << "# Nets           : " << _nets.size()  << '\n'
      << "# Arcs           : " << _arcs.size()  << '\n'
      << "# SCCs           : " << _sccs.size()  << '\n'
      << "# Tests          : " << _tests.size() << '\n'
      << "# Cells          : " << num_cells     << '\n';

  return oss.str();
}

// Function: dump_net_load
std::string Timer::dump_net_load() const {
  std::shared_lock lock(_mutex);
  return _dump_net_load();
}

// Function: _dump_net_load
std::string Timer::_dump_net_load() const {

  std::ostringstream oss;
  
  oss << "Net Load [nets:" << _nets.size() 
      << "]\n";


  if(!_nets.empty())  {

    // find the maximum net name
    auto nlen = _max_net_name_size();

    oss << std::setfill('-') << std::setw(49 + nlen) << '\n'
        << std::setfill(' ') << std::setw(10) << "E/R"  
                             << std::setw(12) << "E/F"
                             << std::setw(12) << "L/R"
                             << std::setw(12) << "L/F" 
                             << std::setw(2 + nlen)  << "Net"   << '\n'
        << std::setfill('-') << std::setw(49 + nlen) << '\n';

    oss << std::setfill(' ') << std::fixed << std::setprecision(3);
    for(const auto& kvp : _nets) {

      const auto& net = kvp.second;
      
      FOR_EACH_EL_RF(el, rf) {
        oss << std::setw(10) << net._load(el, rf) << "  ";
      }

      oss << std::setw(nlen) << net._name << '\n';
    }
    oss << std::setfill('-') << std::setw(49 + nlen) << '\n';
  }

  return oss.str();
}

// Function: dump_pin_cap
std::string Timer::dump_pin_cap() const {
  std::shared_lock lock(_mutex);
  return _dump_pin_cap();
}

// Function: _dump_pin_cap
std::string Timer::_dump_pin_cap() const {

  std::ostringstream oss;
  
  oss << "Pin Capacitance [pins:" << _pins.size() 
      << "]\n";

  if(!_pins.empty())  {

    // find the maximum pin name
    auto plen = _max_pin_name_size();

    oss << std::setfill('-') << std::setw(49 + plen) << '\n'
        << std::setfill(' ') << std::setw(10) << "E/R"  
                             << std::setw(12) << "E/F"
                             << std::setw(12) << "L/R"
                             << std::setw(12) << "L/F" 
                             << std::setw(2 + plen)  << "Pin"   << '\n'
        << std::setfill('-') << std::setw(49 + plen) << '\n';

    oss << std::setfill(' ') << std::fixed << std::setprecision(3);
    for(const auto& kvp : _pins) {

      const auto& pin = kvp.second;
      
      FOR_EACH_EL_RF(el, rf) {
        oss << std::setw(10) << pin.cap(el, rf) << "  ";
      }

      oss << std::setw(plen) << pin._name << '\n';
    }
    oss << std::setfill('-') << std::setw(49 + plen) << '\n';
  }

  return oss.str();
}

// Function: dump_slew
std::string Timer::dump_slew() const {
  std::shared_lock lock(_mutex);
  return _dump_slew();
}

// Function: _dump_slew
std::string Timer::_dump_slew() const {

  std::ostringstream oss;
  
  oss << "Slew [pins:" << _pins.size() << "]\n";

  if(!_pins.empty())  {

    // find the maximum pin name
    auto plen = _max_pin_name_size();

    oss << std::setfill('-') << std::setw(49 + plen) << '\n'
        << std::setfill(' ') << std::setw(10) << "E/R"  
                             << std::setw(12) << "E/F"
                             << std::setw(12) << "L/R"
                             << std::setw(12) << "L/F" 
                             << std::setw(2 + plen)  << "Pin"   << '\n'
        << std::setfill('-') << std::setw(49 + plen) << '\n';

    oss << std::setfill(' ') << std::fixed << std::setprecision(3);
    for(const auto& kvp : _pins) {

      const auto& pin = kvp.second;
      
      FOR_EACH_EL_RF(el, rf) {
        oss << std::setw(10);
        if(auto slew = pin.slew(el, rf); slew) oss << *slew;
        else oss << "n/a";
        oss << "  ";
      }

      oss << std::setw(plen) << pin._name << '\n';
    }
    oss << std::setfill('-') << std::setw(49 + plen) << '\n';
  }

  return oss.str();
}

// Function: dump_slack
std::string Timer::dump_slack() const {
  std::shared_lock lock(_mutex);
  return _dump_slack();
}

// Function: _dump_slack
std::string Timer::_dump_slack() const {

  std::ostringstream oss;
  
  oss << "Slack [pins:" << _pins.size() << "]\n";

  if(!_pins.empty())  {

    // find the maximum pin name
    auto plen = _max_pin_name_size();

    oss << std::setfill('-') << std::setw(49 + plen) << '\n'
        << std::setfill(' ') << std::setw(10) << "E/R"  
                             << std::setw(12) << "E/F"
                             << std::setw(12) << "L/R"
                             << std::setw(12) << "L/F" 
                             << std::setw(2 + plen)  << "Pin"   << '\n'
        << std::setfill('-') << std::setw(49 + plen) << '\n';

    oss << std::setfill(' ') << std::fixed << std::setprecision(3);
    for(const auto& kvp : _pins) {

      const auto& pin = kvp.second;
      
      FOR_EACH_EL_RF(el, rf) {
        oss << std::setw(10);
        if(auto slack = pin.slack(el, rf); slack) oss << *slack;
        else oss << "n/a";
        oss << "  ";
      }

      oss << std::setw(plen) << pin._name << '\n';
    }
    oss << std::setfill('-') << std::setw(49 + plen) << '\n';
  }

  return oss.str();
}

// Function: dump_at
std::string Timer::dump_at() const {
  std::shared_lock lock(_mutex);
  return _dump_at();
}

// Function: _dump_at
std::string Timer::_dump_at() const {

  std::ostringstream oss;
  
  oss << "Arrival time [pins:" << _pins.size() << "]\n";

  if(!_pins.empty())  {

    // find the maximum pin name
    auto plen = _max_pin_name_size();

    oss << std::setfill('-') << std::setw(49 + plen) << '\n'
        << std::setfill(' ') << std::setw(10) << "E/R"  
                             << std::setw(12) << "E/F"
                             << std::setw(12) << "L/R"
                             << std::setw(12) << "L/F" 
                             << std::setw(2 + plen)  << "Pin"   << '\n'
        << std::setfill('-') << std::setw(49 + plen) << '\n';

    oss << std::setfill(' ') << std::fixed << std::setprecision(3);
    for(const auto& kvp : _pins) {

      const auto& pin = kvp.second;
      
      FOR_EACH_EL_RF(el, rf) {
        oss << std::setw(10);
        if(auto at = pin.at(el, rf); at) oss << *at;
        else oss << "n/a";
        oss << "  ";
      }

      oss << std::setw(plen) << pin._name << '\n';
    }
    oss << std::setfill('-') << std::setw(49 + plen) << '\n';
  }

  return oss.str();
}

// Function: dump_rat
std::string Timer::dump_rat() const {
  std::shared_lock lock(_mutex);
  return _dump_rat();
}

// Function: _dump_rat
std::string Timer::_dump_rat() const {

  std::ostringstream oss;
  
  oss << "Required arrival time [pins:" << _pins.size() 
      << "]\n";

  if(!_pins.empty())  {

    // find the maximum pin name
    auto plen = _max_pin_name_size();

    oss << std::setfill('-') << std::setw(49 + plen) << '\n'
        << std::setfill(' ') << std::setw(10) << "E/R"  
                             << std::setw(12) << "E/F"
                             << std::setw(12) << "L/R"
                             << std::setw(12) << "L/F" 
                             << std::setw(2 + plen)  << "Pin"   << '\n'
        << std::setfill('-') << std::setw(49 + plen) << '\n';

    oss << std::setfill(' ') << std::fixed << std::setprecision(3);
    for(const auto& kvp : _pins) {

      const auto& pin = kvp.second;
      
      FOR_EACH_EL_RF(el, rf) {
        oss << std::setw(10);
        if(auto rat = pin.rat(el, rf); rat) oss << *rat;
        else oss << "n/a";
        oss << "  ";
      }

      oss << std::setw(plen) << pin._name << '\n';
    }
    oss << std::setfill('-') << std::setw(49 + plen) << '\n';
  }

  return oss.str();
}

// Function: dump_cell
std::string Timer::dump_cell(const std::string& name, Split el) const {
  std::shared_lock lock(_mutex);
  return _dump_cell(name, el);
}

// Function: _dump_cell
std::string Timer::_dump_cell(const std::string& name, Split el) const {
  if(_celllib[el]) {
    std::ostringstream oss;
    if(auto ptr = _celllib[el]->cell(name); ptr) {
      oss << *ptr;
    }
    else {
      oss << "cell not found\n";
    }
    return oss.str();
  }
  else {
    return "celllib not found\n";
  }
}

// Function: dump_celllib
std::string Timer::dump_celllib(Split el) const {
  std::shared_lock lock(_mutex);
  return _dump_celllib(el);
}

// Function: _dump_celllib
std::string Timer::_dump_celllib(Split el) const {
  
  if(_celllib[el]) {
    std::ostringstream oss;
    oss << *_celllib[el];
    return oss.str();
  }
  else {
    return "celllib not found\n";
  }
}
    
};  // end of namespace ot. -----------------------------------------------------------------------


