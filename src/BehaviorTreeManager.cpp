/* Copyright [2022] <theItalianJob>
 * Author: Gerardo Puga
 */

// standard library
#include <memory>
#include <utility>

// project
#include <behavior_tree_extras/BTExecutionResult.hpp>
#include <behavior_tree_extras/BehaviorTreeManager.hpp>

namespace behavior_tree_extras
{
BehaviorTreeManager::BehaviorTreeManager(BTHandle bt_handle) : bt_handle_{ std::move(bt_handle) }
{
}

BehaviorTreeManager::~BehaviorTreeManager()
{
}

BTExecutionResult BehaviorTreeManager::run()
{
  BT::NodeStatus tree_status = BT::NodeStatus::RUNNING;

  halted_ = false;
  // fast track, if tickRoot() does not return RUNNING, we never sleep and terminate quickly
  tree_status = bt_handle_.tree->tickRoot();
  // slow track, if we enter the loop we wait a tick interval before calling back.
  while ((tree_status == BT::NodeStatus::RUNNING) && !halted_)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
    tree_status = bt_handle_.tree->tickRoot();
  }

  return tree_status == BT::NodeStatus::SUCCESS ? BTExecutionResult::SUCCESS :
                                                  BTExecutionResult::FAILURE;
}

void BehaviorTreeManager::halt()
{
  halted_ = true;
  bt_handle_.tree->haltTree();
}

}  // namespace behavior_tree_extras
