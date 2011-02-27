cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

PROJECT(BeliefPropagation)
#
# ADD_EXECUTABLE(TestBinaryDenoisingSynthetic TestBinaryDenoisingSynthetic.cxx
# BinaryDenoising.cxx
# UpdateSchedule.cxx RasterAllNeighborsUpdateSchedule.cxx RasterOneNeighborUpdateSchedule.cxx RandomUpdateSchedule.cxx PriorityUpdateSchedule.cxx
# Message.cxx
# Helpers.cxx  Debugging.cxx)
# TARGET_LINK_LIBRARIES(TestBinaryDenoisingSynthetic ITKCommon ITKIO ITKBasicFilters)

ADD_EXECUTABLE(TestBinaryDenoisingReal TestBinaryDenoisingReal.cxx
BinaryDenoising.cxx Node.cxx MessageVector.cxx Comparisons.cxx
UpdateSchedule.cxx RasterAllNeighborsUpdateSchedule.cxx RasterOneNeighborUpdateSchedule.cxx RandomUpdateSchedule.cxx RandomUniqueUpdateSchedule.cxx
Message.cxx
Helpers.cxx  Debugging.cxx)
TARGET_LINK_LIBRARIES(TestBinaryDenoisingReal ITKCommon ITKIO ITKBasicFilters)
#
# ADD_EXECUTABLE(TestGrayscaleInpainting TestGrayscaleInpainting.cxx
# GrayscaleInpainting.cxx
# UpdateSchedule.cxx RasterAllNeighborsUpdateSchedule.cxx RasterOneNeighborUpdateSchedule.cxx RandomUpdateSchedule.cxx PriorityUpdateSchedule.cxx
# Message.cxx
# Helpers.cxx  Debugging.cxx)
# TARGET_LINK_LIBRARIES(TestGrayscaleInpainting ITKCommon ITKIO ITKBasicFilters)

# ADD_EXECUTABLE(ExampleBinaryDenoising ExampleBinaryDenoising.cxx Helpers.cxx LoopyBP.cxx Debugging.cxx)
# TARGET_LINK_LIBRARIES(ExampleBinaryDenoising ITKCommon ITKIO ITKBasicFilters)

# Tests
# ADD_EXECUTABLE(TestSumProductMessageUpdate TestSumProductMessageUpdate.cxx
# BinaryDenoising.cxx UpdateSchedule.cxx RasterUpdateSchedule.cxx Message.cxx
# Helpers.cxx  Debugging.cxx)
# TARGET_LINK_LIBRARIES(TestSumProductMessageUpdate ITKCommon ITKIO ITKBasicFilters)