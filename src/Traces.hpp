/*
    This file is part of ELMO-2.

    ELMO-2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ELMO-2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with ELMO-2.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
    @file Traces.hpp
    @brief Contains the internal representation of the generated Traces.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include <stdexcept>  // for range_error
#include <vector>     // for vector

#include <iostream>  // TODO: for temp debugging

#ifndef TRACES_HPP
#define TRACES_HPP

namespace ELMO2
{
namespace Internal
{
//! @class Traces
//! @brief The internal representation of the generated Traces for the target
//! program.
//! TODO: Learn more about traces to fill in a detailed description of what this
//! is.
class Traces
{
private:
    // TODO: Need a very large (several GBs) container to store traces. Look
    // into libs? STXXL? Basically a very large array is needed.

    // 2d vector. A vector of traces containing a vector of samples within that
    // trace.
    // Empty braces initialisation prevents a segfault.
    std::vector<std::vector<uint32_t>> m_traces{{}};

public:
    void Append(const uint32_t& p_value)
    {
        std::cout << "Trace: ";
        std::cout << p_value << std::endl;

        m_traces.back().push_back(p_value);
    }

    //! @brief Constructs a new std::vector and add it to m_traces. As traces
    //! are appended to the last element in m_traces, this will mean that new
    //! samples are added to this new trace in future.
    //! @note This only needs to be called after the first trace is added. One
    //! empty trace will be automatically created upon initialisation ready to
    //! be populated.
    void Start_New_Trace()
    {
        // Don't create a new trace if the current is empty - Use that instead,
        if (m_traces.back().empty())
        {
            return;
        }

        if (m_traces.back().size() != m_traces.front().size())
        {
            throw std::range_error("Cannot start a new trace, this trace is "
                                   "not the same length as the other traces");
        }
        m_traces.emplace_back();
    }

    size_t Get_Number_Of_Traces() const { return m_traces.size(); }

    size_t Get_Number_Of_Samples_Per_Trace() const
    {
        return m_traces[0].size();
    }
};
}  // namespace Internal
}  // namespace ELMO2

#endif
