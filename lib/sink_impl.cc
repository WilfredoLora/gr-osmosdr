/* -*- c++ -*- */
/*
 * Copyright 2013 Dimitri Stolnikov <horiz0n@gmx.net>
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * config.h is generated by configure.  It contains the results
 * of probing for features, options etc.  It should be the first
 * file included in your .cc file.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/constants.h>

#ifdef ENABLE_UHD
#include "uhd_sink_c.h"
#endif
#ifdef ENABLE_HACKRF
#include "hackrf_sink_c.h"
#endif
#ifdef ENABLE_BLADERF
#include "bladerf_sink_c.h"
#endif
#ifdef ENABLE_SOAPY
#include "soapy_sink_c.h"
#endif
#ifdef ENABLE_REDPITAYA
#include "redpitaya_sink_c.h"
#endif
#ifdef ENABLE_FREESRP
#include <freesrp_sink_c.h>
#endif
#ifdef ENABLE_XTRX
#include "xtrx_sink_c.h"
#endif
#ifdef ENABLE_FILE
#include "file_sink_c.h"
#endif

#include "arg_helpers.h"
#include "sink_impl.h"

/*
 * Create a new instance of sink_impl and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
osmosdr::sink::sptr
osmosdr::sink::make( const std::string &args )
{
  return gnuradio::get_initial_sptr( new sink_impl(args) );
}

/*
 * The private constructor
 */
sink_impl::sink_impl( const std::string &args )
  : gr::hier_block2 ("sink_impl",
        args_to_io_signature(args),
        gr::io_signature::make(0, 0, 0)),
    _sample_rate(NAN)
{
  size_t channel = 0;
  bool device_specified = false;

  std::vector< std::string > arg_list = args_to_vector(args);

  std::vector< std::string > dev_types;

#ifdef ENABLE_UHD
  dev_types.push_back("uhd");
#endif
#ifdef ENABLE_HACKRF
  dev_types.push_back("hackrf");
#endif
#ifdef ENABLE_BLADERF
  dev_types.push_back("bladerf");
#endif
#ifdef ENABLE_SOAPY
  dev_types.push_back("soapy");
#endif
#ifdef ENABLE_REDPITAYA
  dev_types.push_back("redpitaya");
#endif
#ifdef ENABLE_FREESRP
  dev_types.push_back("freesrp");
#endif
#ifdef ENABLE_XTRX
  dev_types.push_back("xtrx");
#endif
#ifdef ENABLE_FILE
  dev_types.push_back("file");
#endif

  std::cerr << "gr-osmosdr "
            << GR_OSMOSDR_VERSION << " (" << GR_OSMOSDR_LIBVER << ") "
            << "gnuradio " << gr::version() << std::endl;
  std::cerr << "built-in sink types: ";
  BOOST_FOREACH(std::string dev_type, dev_types)
    std::cerr << dev_type << " ";
  std::cerr << std::endl;

  BOOST_FOREACH(std::string arg, arg_list) {
    dict_t dict = params_to_dict(arg);
    BOOST_FOREACH(std::string dev_type, dev_types) {
      if ( dict.count( dev_type ) ) {
        device_specified = true;
        break;
      }
    }
  }

  if ( ! device_specified ) {
    std::vector< std::string > dev_list;
#ifdef ENABLE_UHD
    BOOST_FOREACH( std::string dev, uhd_sink_c::get_devices() )
      dev_list.push_back( dev );
#endif
#ifdef ENABLE_BLADERF
    BOOST_FOREACH( std::string dev, bladerf_sink_c::get_devices() )
      dev_list.push_back( dev );
#endif
#ifdef ENABLE_HACKRF
    BOOST_FOREACH( std::string dev, hackrf_sink_c::get_devices() )
      dev_list.push_back( dev );
#endif
#ifdef ENABLE_SOAPY
    BOOST_FOREACH( std::string dev, soapy_sink_c::get_devices() )
      dev_list.push_back( dev );
#endif
#ifdef ENABLE_REDPITAYA
    BOOST_FOREACH( std::string dev, redpitaya_sink_c::get_devices() )
      dev_list.push_back( dev );
#endif
#ifdef ENABLE_FREESRP
    BOOST_FOREACH( std::string dev, freesrp_sink_c::get_devices() )
      dev_list.push_back( dev );
#endif
#ifdef ENABLE_XTRX
	BOOST_FOREACH( std::string dev, xtrx_sink_c::get_devices() )
#endif
#ifdef ENABLE_FILE
    BOOST_FOREACH( std::string dev, file_sink_c::get_devices() )
      dev_list.push_back( dev );
#endif

//    std::cerr << std::endl;
//    BOOST_FOREACH( std::string dev, dev_list )
//      std::cerr << "'" << dev << "'" << std::endl;

    if ( dev_list.size() )
      arg_list.push_back( dev_list.front() );
    else
      throw std::runtime_error("No supported devices found (check the connection and/or udev rules).");
  }

  BOOST_FOREACH(std::string arg, arg_list) {

    dict_t dict = params_to_dict(arg);

//    std::cerr << std::endl;
//    BOOST_FOREACH( dict_t::value_type &entry, dict )
//      std::cerr << "'" << entry.first << "' = '" << entry.second << "'" << std::endl;

    sink_iface *iface = NULL;
    gr::basic_block_sptr block;

#ifdef ENABLE_UHD
    if ( dict.count("uhd") ) {
      uhd_sink_c_sptr sink = make_uhd_sink_c( arg );
      block = sink; iface = sink.get();
    }
#endif
#ifdef ENABLE_HACKRF
    if ( dict.count("hackrf") ) {
      hackrf_sink_c_sptr sink = make_hackrf_sink_c( arg );
      block = sink; iface = sink.get();
    }
#endif
#ifdef ENABLE_BLADERF
    if ( dict.count("bladerf") ) {
      bladerf_sink_c_sptr sink = make_bladerf_sink_c( arg );
      block = sink; iface = sink.get();
    }
#endif
#ifdef ENABLE_SOAPY
    if ( dict.count("soapy") ) {
      soapy_sink_c_sptr sink = make_soapy_sink_c( arg );
      block = sink; iface = sink.get();
    }
#endif
#ifdef ENABLE_REDPITAYA
    if ( dict.count("redpitaya") ) {
      redpitaya_sink_c_sptr sink = make_redpitaya_sink_c( arg );
      block = sink; iface = sink.get();
    }
#endif
#ifdef ENABLE_FREESRP
    if ( dict.count("freesrp") ) {
      freesrp_sink_c_sptr sink = make_freesrp_sink_c( arg );
      block = sink; iface = sink.get();
    }
#endif
#ifdef ENABLE_XTRX
    if ( dict.count("xtrx") ) {
      xtrx_sink_c_sptr sink = make_xtrx_sink_c( arg );
      block = sink; iface = sink.get();
    }
#endif
#ifdef ENABLE_FILE
    if ( dict.count("file") ) {
      file_sink_c_sptr sink = make_file_sink_c( arg );
      block = sink; iface = sink.get();
    }
#endif

    if ( iface != NULL && long(block.get()) != 0 ) {
      _devs.push_back( iface );

      for (size_t i = 0; i < iface->get_num_channels(); i++) {
        connect(self(), channel++, block, i);
      }
    } else if ( (iface != NULL) || (long(block.get()) != 0) )
      throw std::runtime_error("Either iface or block are NULL.");

  }

  if (!_devs.size())
    throw std::runtime_error("No devices specified via device arguments.");
}

size_t sink_impl::get_num_channels()
{
  size_t channels = 0;

  BOOST_FOREACH( sink_iface *dev, _devs )
    channels += dev->get_num_channels();

  return channels;
}

#define NO_DEVICES_MSG  "FATAL: No device(s) available to work with."

osmosdr::meta_range_t sink_impl::get_sample_rates()
{
  if ( ! _devs.empty() )
    return _devs[0]->get_sample_rates(); // assume same devices used in the group
#if 0
  else
    throw std::runtime_error(NO_DEVICES_MSG);
#endif
  return osmosdr::meta_range_t();
}

double sink_impl::set_sample_rate(double rate)
{
  double sample_rate = 0;

  if (_sample_rate != rate) {
#if 0
    if (_devs.empty())
      throw std::runtime_error(NO_DEVICES_MSG);
#endif
    BOOST_FOREACH( sink_iface *dev, _devs )
      sample_rate = dev->set_sample_rate(rate);

    _sample_rate = sample_rate;
  }

  return sample_rate;
}

double sink_impl::get_sample_rate()
{
  double sample_rate = 0;

  if (!_devs.empty())
    sample_rate = _devs[0]->get_sample_rate(); // assume same devices used in the group
#if 0
  else
    throw std::runtime_error(NO_DEVICES_MSG);
#endif
  return sample_rate;
}

osmosdr::freq_range_t sink_impl::get_freq_range( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_freq_range( dev_chan );

  return osmosdr::freq_range_t();
}

double sink_impl::set_center_freq( double freq, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ ) {
        if ( _center_freq[ chan ] != freq ) {
          _center_freq[ chan ] = freq;
          return dev->set_center_freq( freq, dev_chan );
        } else { return _center_freq[ chan ]; }
      }

  return 0;
}

double sink_impl::get_center_freq( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_center_freq( dev_chan );

  return 0;
}

double sink_impl::set_freq_corr( double ppm, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ ) {
        if ( _freq_corr[ chan ] != ppm ) {
          _freq_corr[ chan ] = ppm;
          return dev->set_freq_corr( ppm, dev_chan );
        } else { return _freq_corr[ chan ]; }
      }

  return 0;
}

double sink_impl::get_freq_corr( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_freq_corr( dev_chan );

  return 0;
}

std::vector<std::string> sink_impl::get_gain_names( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain_names( dev_chan );

  return std::vector< std::string >();
}

osmosdr::gain_range_t sink_impl::get_gain_range( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain_range( dev_chan );

  return osmosdr::gain_range_t();
}

osmosdr::gain_range_t sink_impl::get_gain_range( const std::string & name, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain_range( name, dev_chan );

  return osmosdr::gain_range_t();
}

bool sink_impl::set_gain_mode( bool automatic, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ ) {
        if ( _gain_mode[ chan ] != automatic ) {
          _gain_mode[ chan ] = automatic;
          bool mode = dev->set_gain_mode( automatic, dev_chan );
          if (!automatic) // reapply gain value when switched to manual mode
            dev->set_gain( _gain[ chan ], dev_chan );
          return mode;
        } else { return _gain_mode[ chan ]; }
      }

  return false;
}

bool sink_impl::get_gain_mode( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain_mode( dev_chan );

  return false;
}

double sink_impl::set_gain( double gain, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ ) {
        if ( _gain[ chan ] != gain ) {
          _gain[ chan ] = gain;
          return dev->set_gain( gain, dev_chan );
        } else { return _gain[ chan ]; }
      }

  return 0;
}

double sink_impl::set_gain( double gain, const std::string & name, size_t chan)
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->set_gain( gain, name, dev_chan );

  return 0;
}

double sink_impl::get_gain( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain( dev_chan );

  return 0;
}

double sink_impl::get_gain( const std::string & name, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_gain( name, dev_chan );

  return 0;
}

double sink_impl::set_if_gain( double gain, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ ) {
        if ( _if_gain[ chan ] != gain ) {
          _if_gain[ chan ] = gain;
          return dev->set_if_gain( gain, dev_chan );
        } else { return _if_gain[ chan ]; }
      }

  return 0;
}

double sink_impl::set_bb_gain( double gain, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ ) {
        if ( _bb_gain[ chan ] != gain ) {
          _bb_gain[ chan ] = gain;
          return dev->set_bb_gain( gain, dev_chan );
        } else { return _bb_gain[ chan ]; }
      }

  return 0;
}

std::vector< std::string > sink_impl::get_antennas( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_antennas( dev_chan );

  return std::vector< std::string >();
}

std::string sink_impl::set_antenna( const std::string & antenna, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ ) {
        if ( _antenna[ chan ] != antenna ) {
          _antenna[ chan ] = antenna;
          return dev->set_antenna( antenna, dev_chan );
        } else { return _antenna[ chan ]; }
      }

  return "";
}

std::string sink_impl::get_antenna( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_antenna( dev_chan );

  return "";
}

void sink_impl::set_dc_offset( const std::complex<double> &offset, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        dev->set_dc_offset( offset, dev_chan );
}

void sink_impl::set_iq_balance( const std::complex<double> &balance, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        dev->set_iq_balance( balance, dev_chan );
}

double sink_impl::set_bandwidth( double bandwidth, size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ ) {
        if ( _bandwidth[ chan ] != bandwidth || 0.0f == bandwidth ) {
          _bandwidth[ chan ] = bandwidth;
          return dev->set_bandwidth( bandwidth, dev_chan );
        } else { return _bandwidth[ chan ]; }
      }

  return 0;
}

double sink_impl::get_bandwidth( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_bandwidth( dev_chan );

  return 0;
}

osmosdr::freq_range_t sink_impl::get_bandwidth_range( size_t chan )
{
  size_t channel = 0;
  BOOST_FOREACH( sink_iface *dev, _devs )
    for (size_t dev_chan = 0; dev_chan < dev->get_num_channels(); dev_chan++)
      if ( chan == channel++ )
        return dev->get_bandwidth_range( dev_chan );

  return osmosdr::freq_range_t();
}

void sink_impl::set_time_source(const std::string &source, const size_t mboard)
{
  if (mboard != osmosdr::ALL_MBOARDS){
      _devs.at(mboard)->set_time_source( source );
      return;
  }

  for (size_t m = 0; m < _devs.size(); m++){ /* propagate ALL_MBOARDS */
      _devs.at(m)->set_time_source( source, osmosdr::ALL_MBOARDS );
  }
}

std::string sink_impl::get_time_source(const size_t mboard)
{
  return _devs.at(mboard)->get_time_source( mboard );
}

std::vector<std::string> sink_impl::get_time_sources(const size_t mboard)
{
  return _devs.at(mboard)->get_time_sources( mboard );
}

void sink_impl::set_clock_source(const std::string &source, const size_t mboard)
{
  if (mboard != osmosdr::ALL_MBOARDS){
      _devs.at(mboard)->set_clock_source( source );
      return;
  }

  for (size_t m = 0; m < _devs.size(); m++){ /* propagate ALL_MBOARDS */
      _devs.at(m)->set_clock_source( source, osmosdr::ALL_MBOARDS );
  }
}

std::string sink_impl::get_clock_source(const size_t mboard)
{
  return _devs.at(mboard)->get_clock_source( mboard );
}

std::vector<std::string> sink_impl::get_clock_sources(const size_t mboard)
{
  return _devs.at(mboard)->get_clock_sources( mboard );
}

double sink_impl::get_clock_rate(size_t mboard)
{
  return _devs.at(mboard)->get_clock_rate( mboard );
}

void sink_impl::set_clock_rate(double rate, size_t mboard)
{
  if (mboard != osmosdr::ALL_MBOARDS){
      _devs.at(mboard)->set_clock_rate( rate );
      return;
  }

  for (size_t m = 0; m < _devs.size(); m++){ /* propagate ALL_MBOARDS */
      _devs.at(m)->set_clock_rate( rate, osmosdr::ALL_MBOARDS );
  }
}

osmosdr::time_spec_t sink_impl::get_time_now(size_t mboard)
{
  return _devs.at(mboard)->get_time_now( mboard );
}

osmosdr::time_spec_t sink_impl::get_time_last_pps(size_t mboard)
{
  return _devs.at(mboard)->get_time_last_pps( mboard );
}

void sink_impl::set_time_now(const osmosdr::time_spec_t &time_spec, size_t mboard)
{
  if (mboard != osmosdr::ALL_MBOARDS){
      _devs.at(mboard)->set_time_now( time_spec );
      return;
  }

  for (size_t m = 0; m < _devs.size(); m++){ /* propagate ALL_MBOARDS */
      _devs.at(m)->set_time_now( time_spec, osmosdr::ALL_MBOARDS );
  }
}

void sink_impl::set_time_next_pps(const osmosdr::time_spec_t &time_spec)
{
  BOOST_FOREACH( sink_iface *dev, _devs )
  {
    dev->set_time_next_pps( time_spec );
  }
}

void sink_impl::set_time_unknown_pps(const osmosdr::time_spec_t &time_spec)
{
  BOOST_FOREACH( sink_iface *dev, _devs )
  {
    dev->set_time_unknown_pps( time_spec );
  }
}
