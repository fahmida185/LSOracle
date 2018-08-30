#include <ot/shell/shell.hpp>

namespace ot {

// Procedure: exec_ops
void Shell::_exec_ops() {
  OT_LOGW(std::quoted("exec_ops"), " is obselete and has no effect");
}

// Procedure: init_timer
void Shell::_init_timer() {
  OT_LOGW(std::quoted("init_timer"), " is obselete and has no effect");
}

// Procedure: set_early_celllib_fpath
void Shell::_set_early_celllib_fpath() {
  OT_LOGW(
    std::quoted("set_early_celllib_fpath"), " is obselete; use ", std::quoted("read_celllib") 
  );
  if(std::filesystem::path path; _is >> path) {
    _timer.read_celllib(std::move(path), EARLY);
  }
}

// Procedure: set_late_celllib_fpath
void Shell::_set_late_celllib_fpath() {
  OT_LOGW(
    std::quoted("set_late_celllib_fpath"), " is obselete; use ", std::quoted("read_celllib")
  );
  if(std::filesystem::path path; _is >> path) {
    _timer.read_celllib(std::move(path), LATE);
  }
}

// Procedure: set_verilog_fpath
void Shell::_set_verilog_fpath() {
  OT_LOGW(
    std::quoted("set_verilog_fpath"), " is obselete; use ", std::quoted("read_verilog")
  );
  if(std::filesystem::path path; _is >> path) {
    _timer.read_verilog(std::move(path));
  }
}

// Procedure: set_spef_fpath
void Shell::_set_spef_fpath() {
  OT_LOGW(
    std::quoted("set_spef_fpath"), " is obselete; use ", std::quoted("read_spef")
  );
  if(std::filesystem::path path; _is >> path) {
    _timer.read_spef(std::move(path));
  }
}

// Procedure: set_timing_fpath
void Shell::_set_timing_fpath() {
  OT_LOGW(
    std::quoted("set_timing_fpath"), " is obselete; use ", std::quoted("read_timing")
  );
  if(std::filesystem::path path; _is >> path) {
    _timer.read_timing(std::move(path));
  }
}

// Procedure: report_timer
void Shell::_report_timer() {

  OT_LOGW(
    std::quoted("report_timer"), " is obselete; use ", std::quoted("dump_timer")
  );
    
  if(auto paths = _timer.worst_paths(1); !paths.empty()) {
    _os << paths[0];
  }
  else {
    _os << "no paths found\n";
  }
}

};  // end of namespace ot. -----------------------------------------------------------------------




