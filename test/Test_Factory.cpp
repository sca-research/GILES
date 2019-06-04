/*
    This file is part of GILES.

    GILES is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GILES is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with GILES.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
    @file Test_Validator_Coefficients.cpp
    @brief Contains the tests for the Validator_Coefficients class.
    @author Scott Egerton
    @date 2017-2018
    @copyright GNU Affero General Public License Version 3+
*/

#include <typeinfo>

#include <catch.hpp>  // for catch

#include <nlohmann/json.hpp>  // for json

#include "Abstract_Factory.hpp"
#include "Abstract_Factory_Register.hpp"
#include "Execution.hpp"

TEST_CASE("Factory pattern testing"
          "[factory]")
{
    class Abstract_Base
    {
    };

    struct Abstract_Derived
        : public Abstract_Base,
          public GILES::Internal::Abstract_Factory_Register<Abstract_Base,
                                                            Abstract_Derived>
    {
        Abstract_Derived()
        {
            // This statement registers this class in the factory, allowing
            // access from elsewhere. Do not delete this or else this class
            // will not appear in the factory. If you wish to make this
            // class inaccessible, a better method would be to remove the
            // corresponding cpp file from the build script. This is
            // required to be "used" somewhere in order to prevent the
            // compiler from optimising it away, thus preventing self
            // registration.
            // Section 6.6.4.1, point 2 of the linked document states that
            // this statement will not be optimised away.
            // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4713.pdf
            // The void cast does nothing functionally but prevents the
            // compiler warning about an unused result.
            (void)m_is_registered;
        }

        static const std::string Get_Name() { return "Abstract_Derived"; }
    };

    using Factory_t = GILES::Internal::Abstract_Factory<Abstract_Base>;

    SECTION("Abstract factory auto registration")
    {
        const auto registered = Factory_t::Get_All();

        // Ensure it was successfully added to the factory.
        REQUIRE(registered.size() == 1);
    }

    SECTION("Abstract factory construction")
    {
        const auto object = Factory_t::Construct("Abstract_Derived");

        // Check we can construct using the factory and that we end up with
        // something of the expected type.
        REQUIRE(typeid(Abstract_Base) == typeid(*object));
    }

    SECTION("Abstract factory find")
    {
        // Check if it can be found by name. This function should never return a
        // nullptr. It will exit in the case of failure but that will also cause
        // this to fail.
        REQUIRE(nullptr != Factory_t::Find("Abstract_Derived"));
    }
}
