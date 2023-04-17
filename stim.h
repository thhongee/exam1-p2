#ifndef _STIM_H_
#define _STIM_H_

#include <iomanip>
using std::setw;
#include "decimation.h"

// WIDTH is defined in decimation.h
const int DEPTH = (1 << WIDTH);

SC_MODULE(stim)
{

  sc_in_clk i_clk;
  sc_out<bool> o_rst;
  sc_fifo_out<sc_uint<WIDTH>> o_a;
  sc_fifo_out<sc_uint<WIDTH>> o_b;
  sc_fifo_out<sc_uint<WIDTH>> o_c;
  sc_fifo_in<sc_uint<WIDTH + 1>> i_sum;
  // Store the previous inputs to FIFOs
  sc_uint<WIDTH> t_a;
  sc_uint<WIDTH> t_b;
  sc_uint<WIDTH> t_c;
  sc_uint<WIDTH + 1> t_sum;

  void stim_gen()
  {
    cout << setw(12) << "time" << setw(12) << "a" << setw(12) << "b" << endl;
    for (int a = 0; a < DEPTH; a++)
    {
      for (int b = 0; b < DEPTH; b++)
      {
        for (int c = 0; c < DEPTH; b++)
        {
          o_a.write(a);
          o_b.write(b);
          o_b.write(c);
          t_a = a;
          t_b = b;
          t_c = c;
          cout << setw(12) << sc_time_stamp();
          cout << setw(12) << t_a.to_string(SC_BIN);
          cout << setw(12) << t_b.to_string(SC_BIN);
          cout << setw(12) << t_c.to_string(SC_BIN) << endl;
          wait();
        }
      }
    }
  }

  void sink()
  {
    cout << setw(12) << "time" << setw(12) << "sum" << endl;
    while (true)
    {
      t_sum = i_sum.read();
      cout << setw(12) << sc_time_stamp();
      cout << setw(12) << t_sum.to_string(SC_BIN) << endl;
      wait();
    }
  }

  void monitor()
  {
    cout << setw(12) << "time" << setw(12) << "a" << setw(12) << "b" << setw(12) << "c" << setw(12)
         << "sum" << endl;
    while (true)
    {
      wait();
      cout << setw(12) << sc_time_stamp();
      cout << setw(12) << t_a.to_string(SC_BIN);
      cout << setw(12) << t_b.to_string(SC_BIN);
      cout << setw(12) << t_c.to_string(SC_BIN);
      cout << setw(12) << t_sum.to_string(SC_BIN) << endl;
      cout << "======================================================" << endl;
    }
  }

  SC_CTOR(stim)
  {
    SC_THREAD(stim_gen);
    sensitive << i_clk.pos();
    dont_initialize();

    SC_THREAD(sink);
    sensitive << i_clk.pos();

    // SC_THREAD(monitor);
    //  sensitive << i_clk.pos();
  }
};

#endif
