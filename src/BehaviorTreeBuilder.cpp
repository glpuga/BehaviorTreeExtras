/* Copyright [2022] <theItalianJob>
 * Author: Gerardo Puga
 */

// standard library
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// project
#include <behavior_tree_extras/BTHandle.hpp>
#include <behavior_tree_extras/BehaviorTreeBuilder.hpp>
#include <behavior_tree_extras/BehaviorTreeManager.hpp>

namespace behavior_tree_extras
{
BehaviorTreeBuilder::BehaviorTreeBuilder(const FactoryLoaderFunction& factory_loader_function)
  : factory_loader_function_{ factory_loader_function }
{
}

BehaviorTreeBuilder::BehaviorTreeBuilder(const FactoryLoaderFunction& factory_loader_function,
                                         const bool)
  : factory_loader_function_{ factory_loader_function }, builder_instance_{ true }
{
}

void BehaviorTreeBuilder::checkIfWeAreABuilderInstance() const
{
  if (!builder_instance_)
  {
    throw std::runtime_error{
      "Can't call build methods on a seed instance. Call createTree() first."
    };
  }
}

BehaviorTreeBuilderInterface& BehaviorTreeBuilder::createTree()
{
  // return a builder instance
  BehaviorTreeBuilder bi(factory_loader_function_, true);
  child_ = std::make_unique<BehaviorTreeBuilder>(std::move(bi));
  return *child_;
}

BehaviorTreeBuilderInterface&
BehaviorTreeBuilder::addFileDescription(const std::string& description_file_full_path)
{
  checkIfWeAreABuilderInstance();
  if (description_string_ != "" || description_file_full_path_ != "")
  {
    throw std::runtime_error{ "A description has already been provided, cant provide one twice" };
  }
  description_file_full_path_ = description_file_full_path;
  return *this;
}

BehaviorTreeBuilderInterface&
BehaviorTreeBuilder::addStringDescription(const std::string& description_string)
{
  checkIfWeAreABuilderInstance();
  if (description_string_ != "" || description_file_full_path_ != "")
  {
    throw std::runtime_error{ "A description has already been provided, cant provide one twice" };
  }
  description_string_ = description_string;
  return *this;
}

BehaviorTreeBuilderInterface& BehaviorTreeBuilder::addRootName(const std::string& root_name)
{
  checkIfWeAreABuilderInstance();
  if (root_name_ != "")
  {
    throw std::runtime_error{
      "A root node name has already been provided, cant provide one "
      "twice"
    };
  }
  root_name_ = root_name;
  return *this;
}

BehaviorTreeBuilderInterface& BehaviorTreeBuilder::addMockingFlag()
{
  checkIfWeAreABuilderInstance();
  mocking_flag_ = true;
  return *this;
}

BehaviorTreeBuilderInterface& BehaviorTreeBuilder::addBlackboard(BT::Blackboard::Ptr blackboard)
{
  checkIfWeAreABuilderInstance();
  if (blackboard_)
  {
    throw std::runtime_error{ "A blackboard has already been provided, cant provide one twice" };
  }
  blackboard_ = std::move(blackboard);
  return *this;
}

BehaviorTreeBuilderInterface& BehaviorTreeBuilder::addLogger(ModularStatusChangeLogger::Ptr logger)
{
  checkIfWeAreABuilderInstance();
  loggers_.emplace_back(std::move(logger));
  return *this;
}

BehaviorTreeManagerInterface::Ptr BehaviorTreeBuilder::build()
{
  BTHandle bth;

  std::unique_ptr<BT::Tree> tree;

  BT::BehaviorTreeFactory bt_factory;
  factory_loader_function_(bt_factory);

  // if there was no blackboard provided, create a default one.
  if (!blackboard_)
  {
    blackboard_ = BT::Blackboard::create();
  }

  if (description_string_ != "")
  {
    bth.tree = std::make_unique<BT::Tree>(bt_factory.createTreeFromText(
        description_string_, std::move(blackboard_), root_name_, mocking_flag_));
  }
  else if (description_file_full_path_ != "")
  {
    bth.tree = std::make_unique<BT::Tree>(bt_factory.createTreeFromFile(
        description_file_full_path_, std::move(blackboard_), root_name_, mocking_flag_));
  }
  else
  {
    throw std::runtime_error{ "No tree description was provided." };
  }

  // add a single custom logger
  bth.loggers = std::move(loggers_);

  std::for_each(bth.loggers.begin(), bth.loggers.end(),
                [&bth](auto& logger) { logger->attach(*bth.tree); });

  return std::make_unique<BehaviorTreeManager>(std::move(bth));
}

}  // namespace behavior_tree_extras
