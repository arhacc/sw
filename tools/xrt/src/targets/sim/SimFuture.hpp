//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <targets/common/Future.hpp>

class SimTarget;
class MatrixView;

class SimFuture : public Future {
  SimTarget& simTarget_;
  bool done_{false};

public:
  explicit SimFuture(SimTarget& simTarget) : simTarget_(simTarget) {}

  [[nodiscard]] bool isDone() const override;
  void wait() override;
  bool wait(std::chrono::nanoseconds d) override;
  bool wait(std::uint64_t cycles) override;

  void setDone();
};

class SimRegisterFuture : public SimFuture {
public:
  explicit SimRegisterFuture(SimTarget& simTarget) : SimFuture(simTarget) {}
};

class SimRegisterReadFuture final : public SimRegisterFuture {
  std::uint32_t address_;
  std::uint32_t* dataLocation_;

public:
  SimRegisterReadFuture(SimTarget& simTarget, std::uint32_t address, std::uint32_t* dataLocation);

  [[nodiscard]] std::uint32_t getAddress() const;
  [[nodiscard]] std::uint32_t* getDataLocation() const;

};

class SimRegisterWriteFuture : public SimRegisterFuture {
  uint32_t address_;
  uint32_t data_;

public:
  SimRegisterWriteFuture(SimTarget& simTarget, uint32_t address, uint32_t data);

  [[nodiscard]] std::uint32_t getAddress() const;
  [[nodiscard]] std::uint32_t getData() const;
};

class SimMatrixViewFuture : public SimFuture {
public:
  explicit SimMatrixViewFuture(SimTarget& simTarget);
};

class SimMatrixViewReadFuture : public SimMatrixViewFuture {
  std::shared_ptr<MatrixView> view_;

public:
  SimMatrixViewReadFuture(SimTarget& simTarget, const std::shared_ptr<MatrixView>& view);

  const std::shared_ptr<MatrixView>& getMatrixView();
};

class SimMatrixViewWriteFuture : public SimMatrixViewFuture {
  std::shared_ptr<const MatrixView> view_;

public:
  SimMatrixViewWriteFuture(SimTarget& simTarget, const std::shared_ptr<const MatrixView>& view);

  const std::shared_ptr<const MatrixView>& getMatrixView();
};
