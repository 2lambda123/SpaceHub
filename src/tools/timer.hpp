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
 * @file timer.hpp
 *
 * Header file.
 */
#pragma once

#include <chrono>

namespace space::tools {
    /*---------------------------------------------------------------------------*\
          Class Timer Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * High precision timer for benchmark.
     */
    class Timer {
        using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;

       public:
        /**
         * Start the timer.
         */
        void start();

        /**
         * Get the time in milli-second.
         * @return Time duration from start().
         */
        float get_time();

        /**
         * Pause the timer.
         */
        void pause();

        /**
         * Reset the timer.
         */
        void reset();

       private:
        Time start_;
        float duration_{0};
        bool active_{false};
    };
    /*---------------------------------------------------------------------------*\
          Class Timer Implementation
    \*---------------------------------------------------------------------------*/
    void Timer::start() {
        active_ = true;
        start_ = std::chrono::high_resolution_clock::now();
    }

    float Timer::get_time() {
        if (active_) {
            auto now = std::chrono::high_resolution_clock::now();
            auto len = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_);
            return duration_ + static_cast<float>(len.count()) * std::chrono::milliseconds::period::num /
                                   std::chrono::milliseconds::period::den;
        } else
            return duration_;
    }

    void Timer::pause() {
        duration_ = get_time();
        active_ = false;
    }

    void Timer::reset() {
        active_ = false;
        duration_ = 0;
    }
}  // namespace space::tools
