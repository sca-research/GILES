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

#ifndef UNICORN_INTERFACE_HPP
#define UNICORN_INTERFACE_HPP

#include <string>
#include <vector>

#include "Emulator_Interface.hpp"

namespace ELMO2
{
namespace Internal
{
// Doxygen comments are inherited from Emulator_Interface.hpp
class Unicorn_Interface : public virtual ELMO2::Internal::Emulator_Interface
{
public:
    const ELMO2::Internal::Execution Run_Code() override {}
};
}  // namespace Internal
}  // namespace ELMO2

#endif
