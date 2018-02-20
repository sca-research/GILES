#[=[
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
#]=]

# This macro stores the model details in appropriate variables, ready to be
# utilised by the configure_models function.
# The MODEL_ENABLED parameter can be overriden, but defines the default value.
macro(add_model MODEL_NAME MODEL_DESCRIPTION MODEL_ENABLED)

    # Add this model to a list of models to be included
    list(APPEND MODELS ${MODEL_NAME})

    # Record a description of the model for use in the cmake option.
    set("${MODEL_NAME}_DESCRIPTION" ${MODEL_DESCRIPTION})

    # Whether or not this model is enabled by default. This can be overridden.
    # Converts ON/OFF to true/false.
    if(${MODEL_ENABLED})
        set("${MODEL_NAME}_ENABLED" true)
    else()
        set("${MODEL_NAME}_ENABLED" false)
    endif()
endmacro()

# This macro adds the models to the Model_Factory class enabling them to be
# constructed easily.
macro(configure_models)

    # Add each model individually.
    foreach(MODEL ${MODELS})

        # Add a cmake option, allowing this model to be enabled/disabled by
        # default.
        option(Model_${MODEL} ${${MODEL}_DESCRIPTION} ${${MODEL}_ENABLED})

        # Add the code that will generate the model to the model factory class.
        string(APPEND MODEL_FACTORY_CODE "
    if (\"${MODEL}\" == p_model_type)
    {
        model = std::make_unique<ELMO2::Internal::Model_${MODEL}>(p_execution,
                                                                p_coefficients);
    }")

        # The list of include statements within the model factory. Each model
        # added needs to be included.
        string(APPEND MODEL_FACTORY_INCLUDES "
#include \"${CMAKE_CURRENT_SOURCE_DIR}/Model_${MODEL}.hpp\""
            )

        # The list of files to be included in the cmake.
        list(APPEND MODEL_SOURCES Model_${MODEL}.cpp)

        # The list of all models and whether or not they are enabled by default.
        # Ready to be stored in a member of the model factory class.
        string(APPEND MODEL_FACTORY_LIST ",{\"${MODEL}\", ${${MODEL}_ENABLED}}")
    endforeach()

    # The first comma should be removed as it is a comma seperated list.
    string(SUBSTRING ${MODEL_FACTORY_LIST} 1 -1 MODEL_FACTORY_LIST)

    # Insert the generated code into the file.
    configure_file(Model_Factory.cpp.in Model_Factory.cpp)
endmacro()
