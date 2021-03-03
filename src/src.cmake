macro(addFlag flag)
  if(NOT ADDITIONAL_FLAGS MATCHES ${flag})
    set(ADDITIONAL_FLAGS "${ADDITIONAL_FLAGS} ${flag}")
  endif()
endmacro()

addFlag("-lm")

list(APPEND LOCAL_INCLUDE_PATHS
  common
  events
  mainExecutonBlock
  algoritms
  mainDependencies
)

list(APPEND LOCAL_SOURCE_PATHS
  algoritms/AHRS.cpp
  algoritms/AltitudeProvider.cpp
  algoritms/ComplementaryFilter.cpp
  algoritms/Kalman.cpp
  algoritms/OctoEngineControl.cpp
  algoritms/PID.cpp
  algoritms/QuatroEngineControl.cpp
  algoritms/StepingMeanFilter.cpp
  common/ADC.cpp
  common/BatteryObserver.cpp
  common/esdStructure.cpp
  common/MiniDronEngine.cpp
  mainExecutonBlock/EnginePercentValue.cpp
  mainExecutonBlock/FlyTrybeWithAltitude.cpp
  mainExecutonBlock/flyWithAltitudePID.cpp
  mainExecutonBlock/mainFlyTrybe.cpp
  mainExecutonBlock/PIDofAxisXandY.cpp
  mainExecutonBlock/recognizeEngines.cpp
  mainExecutonBlock/TrybeTools.cpp
  mainExecutonBlock/tuneAxisTrybe.cpp
  mainDependencies/processRadioData.cpp
  mainDependencies/secondMain.cpp
)

function(prepend_list var prefix)
    set(temp "")
    foreach(f ${${var}})
        list(APPEND temp "${prefix}${f}")
    endforeach()
    set(${var} "${temp}" PARENT_SCOPE)
endfunction()
