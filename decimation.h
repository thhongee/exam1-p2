#ifndef _DECIMATION_H_
#define _DECIMATION_H_

#include <iostream>
using namespace std;

#include <systemc>
using namespace sc_dt;
using namespace sc_core;

const int WIDTH = 3;

class decimation : public sc_module
{
public:
  sc_in_clk i_clk;
  sc_in<bool> i_rst;
  sc_fifo_in<sc_uint<WIDTH>> i_a_port;
  sc_fifo_in<sc_uint<WIDTH>> i_b_port;
  sc_fifo_in<sc_uint<WIDTH>> i_c_port;
  sc_fifo_out<sc_uint<WIDTH + 1>> o_sum_port;

  void do_decimation()
  {
    const float x_input_signal = {0.500, 0.525, 0.549, 0.574, 0.598, 0.622, 0.646, 0.670,
                                  0.693, 0.715, 0.737, 0.759, 0.780, 0.800, 0.819, 0.838,
                                  0.856, 0.873, 0.889, 0.904, 0.918, 0.931, 0.943, 0.954,
                                  0.964, 0.972, 0.980, 0.986, 0.991, 0.995, 0.998, 1.000,
                                  1.000, 0.999, 0.997, 0.994, 0.989, 0.983, 0.976, 0.968,
                                  0.959, 0.949, 0.937, 0.925, 0.911, 0.896, 0.881, 0.864,
                                  0.847, 0.829, 0.810, 0.790, 0.769, 0.748, 0.726, 0.704,
                                  0.681, 0.658, 0.634, 0.610, 0.586, 0.562, 0.537, 0.512,
                                  0.488, 0.463, 0.438, 0.414, 0.390, 0.366, 0.342, 0.319,
                                  0.296, 0.274, 0.252, 0.231, 0.210, 0.190, 0.171, 0.153,
                                  0.136, 0.119, 0.104, 0.089, 0.075, 0.063, 0.051, 0.041,
                                  0.032, 0.024, 0.017, 0.011, 0.006, 0.003, 0.001, 0.000,
                                  0.000, 0.002, 0.005, 0.009, 0.014, 0.020, 0.028, 0.036,
                                  0.046, 0.057, 0.069, 0.082, 0.096, 0.111, 0.127, 0.144,
                                  0.162, 0.181, 0.200, 0.220, 0.241, 0.263, 0.285, 0.307,
                                  0.330, 0.354, 0.378, 0.402, 0.426, 0.451, 0.475, 0.500};
    while (true)
    {
      for (i = 0; i < 126; i += 2)
      {
        _i_a = x_input_signal[i];
        _i_b = x_input_signal[i + 1];
        _i_c = x_input_signal[i + 2];
        wait();
        _o_sum = _i_a * (1 / 6) + _i_b * (1 / 3) + _i_c * (1 / 2);
        o_sum_port.write(_o_sum);
        wait();
      }
    }
  }

  SC_HAS_PROCESS(decimation);
  decimation(sc_module_name name, int i) : id(i)
  {
    SC_THREAD(do_decimation);
    sensitive << i_clk.pos();
    dont_initialize();
  }

  sc_uint<WIDTH> i_a() { return _i_a; }
  sc_uint<WIDTH> i_b() { return _i_b; }
  sc_uint<WIDTH> i_c() { return _i_c; }
  sc_uint<WIDTH> o_sum() { return _o_sum; }

private:
  const int id;
  sc_uint<WIDTH> _i_a;
  sc_uint<WIDTH> _i_b;
  sc_uint<WIDTH> _i_c;
  sc_uint<WIDTH + 1> _o_sum;
};

#endif
