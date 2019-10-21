/*---------------------------------------------------------------------------*\
        .-''''-.         |
       /        \        |
      /_        _\       |  SpaceHub: The Open Source N-body Toolkit
     // \  <>  / \\      |
     |\__\    /__/|      |  Website:  https://yihanwangastro.github.io/SpaceHub/
      \    ||    /       |
        \  __  /         |  Copyright (C) 2019 Yihan Wang
         '.__.'          |
---------------------------------------------------------------------
License
    This file is part of SpaceHub.
    SpaceHub is free software: you can redistribute it and/or modify it under
    the terms of the MIT License. SpaceHub is distributed in the hope that it
    will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the MIT License
    for more details. You should have received a copy of the MIT License along
    with SpaceHub.
\*---------------------------------------------------------------------------*/
/**
 *
 * @file callbacks.hpp
 *
 * Header file.
 */
#ifndef SPACEHUB_CALLBACKS_HPP
#define SPACEHUB_CALLBACKS_HPP


#include "../tools/timer.hpp"
#include <fstream>
#include <iomanip>
#include <memory>

/**
 * @namespace space::run_operations
 * Documentation for run_operations
 */
namespace space::run_operations {

  /**
   *
   * @tparam Operation
   */
  template<typename Operation>
  class TimeSlice {
  public:
    TimeSlice(Operation const &opt, double start, double end, size_t opt_num = 5000)
            : opt_{opt},
              opt_time_{start},
              end_time_{end},
              opt_interval_{(end - start) / opt_num} {}

    template<typename ParticleSys>
    inline void operator()(ParticleSys &ptc) {
      using Scalar = typename ParticleSys::Scalar;
      auto t = ptc.time();
      if (t >= static_cast<Scalar>(opt_time_) && opt_time_ <= end_time_) {
        opt_(ptc);
        opt_time_ += opt_interval_;
      }
    }

    TimeSlice(TimeSlice &&other) noexcept = default;

    TimeSlice(TimeSlice const &other) = default;

    TimeSlice &operator=(TimeSlice const &) = default;

    TimeSlice &operator=(TimeSlice &&) noexcept = default;

    void reset_slice_params(double start, double end, size_t opt_num = 5000) {
      opt_time_ = start;
      end_time_ = end;
      opt_interval_ = (end - start) / opt_num;
    }

  private:
    Operation opt_;
    double opt_time_{0};
    double end_time_{0};
    double opt_interval_{0};
  };

  template<typename Operation>
  class StepSlice {
  public:
    explicit StepSlice(Operation const &opt, size_t step_interval = 1)
            : opt_{opt},
              step_interval_{step_interval} {}

    template<typename ParticleSys>
    inline void operator()(ParticleSys &ptc) {
      if (step_ % step_interval_ == 0) {
        opt_(ptc);
      }
      step_++;
    }

    StepSlice(StepSlice &&other) noexcept = default;

    StepSlice(StepSlice const &other) = default;

    StepSlice &operator=(StepSlice const &) = default;

    StepSlice &operator=(StepSlice &&) noexcept = default;

    void reset_slice_params(size_t step_interval) {
      step_ = 0;
      step_interval_ = step_interval;
    }

  private:
    Operation opt_;
    size_t step_{0};
    size_t step_interval_{0};
  };

  class DefaultWriter {
  public:
    explicit DefaultWriter(std::string const &file_name)
            : fstream_{std::make_shared<std::ofstream>(file_name)} {
      if (!fstream_->is_open()) {
        spacehub_abort("Fail to open the file " + file_name);
      } else {
        (*fstream_) /*<< std::fixed*/ << std::setprecision(16);
      }
    }

    template<typename ParticleSys>
    inline void operator()(ParticleSys &ptc) {
      *fstream_ << ptc << '\n';
    }

    template<typename T>
    friend DefaultWriter &operator<<(DefaultWriter &wtr, T const &d) {
      (*wtr.fstream_) << d;
      return wtr;
    }

  private:
    std::shared_ptr<std::ofstream> fstream_;
  };
}
#endif

