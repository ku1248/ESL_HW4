#ifndef GAUSSIAN_FILTER_H_
#define GAUSSIAN_FILTER_H_
#include <systemc>
#include <cmath>
#include <iomanip>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "filter_def.h"

struct GaussianFilter : public sc_module {
  tlm_utils::simple_target_socket<GaussianFilter> tsock;

  sc_fifo<unsigned char> i_r;
  sc_fifo<unsigned char> i_g;
  sc_fifo<unsigned char> i_b;
  sc_fifo<int> o_result_r;
  sc_fifo<int> o_result_g;
  sc_fifo<int> o_result_b;

  SC_HAS_PROCESS(GaussianFilter);

  GaussianFilter(sc_module_name n): 
    sc_module(n), 
    tsock("t_skt"), 
    base_offset(0) 
  {
    tsock.register_b_transport(this, &GaussianFilter::blocking_transport);
    SC_THREAD(do_filter);
  }

  ~GaussianFilter() {
	}
  
  unsigned int base_offset;

  void do_filter(){
    { wait(CLOCK_PERIOD, SC_NS); }
    while (true) {
      int tmp_r = 0;
      int tmp_b = 0;
      int tmp_g = 0;
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
          tmp_r += i_r.read() * mask[u][v];
          tmp_g += i_g.read() * mask[u][v];
          tmp_b += i_b.read() * mask[u][v];
          wait(CLOCK_PERIOD, SC_NS);
        }
      }
      double total = 0;
      tmp_r /= 16;
      tmp_g /= 16;
      tmp_b /= 16;
      char val_r = (char)tmp_r;
      char val_g = (char)tmp_g;
      char val_b = (char)tmp_b;
      o_result_r.write(val_r);
      o_result_g.write(val_g);
      o_result_b.write(val_b);
    }
  }

  void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay){
    wait(delay);
    // unsigned char *mask_ptr = payload.get_byte_enable_ptr();
    // auto len = payload.get_data_length();
    tlm::tlm_command cmd = payload.get_command();
    sc_dt::uint64 addr = payload.get_address();
    unsigned char *data_ptr = payload.get_data_ptr();

    addr -= base_offset;


    // cout << (int)data_ptr[0] << endl;
    // cout << (int)data_ptr[1] << endl;
    // cout << (int)data_ptr[2] << endl;
    word buffer;

    switch (cmd) {
      case tlm::TLM_READ_COMMAND:
        // cout << "READ" << endl;
        switch (addr) {
          case GAUSSIAN_FILTER_RESULT_ADDR:
            buffer.uc[0] = o_result_r.read();
            buffer.uc[1] = o_result_g.read();
            buffer.uc[2] = o_result_b.read();
            break;
          default:
            std::cerr << "READ Error! GaussianFilter::blocking_transport: address 0x"
                      << std::setfill('0') << std::setw(8) << std::hex << addr
                      << std::dec << " is not valid" << std::endl;
        }
        data_ptr[0] = buffer.uc[0];
        data_ptr[1] = buffer.uc[1];
        data_ptr[2] = buffer.uc[2];
        data_ptr[3] = buffer.uc[3];
        break;
      case tlm::TLM_WRITE_COMMAND:
        // cout << "WRITE" << endl;
        switch (addr) {
          case GAUSSIAN_FILTER_R_ADDR:
            i_r.write(data_ptr[0]);
            i_g.write(data_ptr[1]);
            i_b.write(data_ptr[2]);
            break;
          default:
            std::cerr << "WRITE Error! GaussianFilter::blocking_transport: address 0x"
                      << std::setfill('0') << std::setw(8) << std::hex << addr
                      << std::dec << " is not valid" << std::endl;
        }
        break;
      case tlm::TLM_IGNORE_COMMAND:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      default:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      }
      payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
  }
};
#endif
