///
/// \file Future.hpp
///
/// \brief Definition of class Future.
#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>

inline std::uint64_t operator""_cycles(const unsigned long long x) {
    return x;
}

///
/// \brief Represents the state of an async IO operation.
///
/// All IO operations return objects of classes derived from this type. The semantics are as follows:
///
/// 1. The IO operation will have been completed when isDone returns true.
/// 2. isDone will always return true after wait has completed successfully.
/// 3. If the IO operation was created using pointers or references to objects, it is the caller's responsibility to
/// make the objects pointed to keep existing until the future returns isDone.
/// 4. It is not guaranteed that isDone will ever return true unless the future is explicitly waited on.
class Future {
  public:
    virtual ~Future();

    ///
    /// \brief Reports if the IO operation has finished.
    ///
    /// \return True if the IO operation has finished.
    [[nodiscard]] virtual bool isDone() const = 0;

    ///
    /// \brief Wait until the IO operation finishes.
    ///
    /// After this, isDone will always return true and does not need to be explicitly checked.
    virtual void wait() = 0;

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::nanoseconds d) = 0;

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is specified in accelerator clock cycles. This is cycle-accurate for the simulator. Otherwise, it
    /// calls the nanosecond version with the argument 10ns * cycles.
    ///
    /// \param cycles Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::uint64_t cycles) {
        return wait(std::chrono::nanoseconds(10) * cycles);
    }

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::microseconds d);

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::milliseconds d);

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::seconds d);

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::minutes d);

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::hours d);

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::days d);

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::weeks d);

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::months d);

    ///
    /// \brief Wait until the IO operation finishes or the timeout expires.
    ///
    /// The duration is advisory. The function may take slightly more time to run even if it fails. It may also return
    /// successfully if the IO operation took slightly more time than the timeout duration.
    ///
    /// \param d Timeout duration.
    /// \return True if the IO operation succeeded. False if the timeout was reached.
    virtual bool wait(std::chrono::years d);
};

///
/// \brief Future which is done when all the specified futures are done.
class AndFuture final : public Future {
    std::vector<std::shared_ptr<Future>> futures_;

  public:
    ///
    /// \brief Construct the AndFuture object.
    ///
    /// A copy of the shared pointers will be copied inside this object.
    ///
    /// \param futures When all these futures will be done, this object will be done.
    explicit AndFuture(std::span<std::shared_ptr<Future>> futures);

    ///
    /// \brief Construct the AndFuture object.
    ///
    /// The specified vector will be stored inside this object.
    ///
    /// \param futures When all these futures will be done, this object will be done.
    explicit AndFuture(std::vector<std::shared_ptr<Future>>&& futures);

    ///
    /// \brief Destroy the AndFuture object.
    ~AndFuture() override;

    ///
    /// \brief Reports if all the specified futures are done.
    ///
    /// \return True if all the futures are done. False if any of the futures are not done.
    [[nodiscard]] bool isDone() const override;

    ///
    /// \brief Waits for all the futures to be done.
    void wait() override;

    ///
    /// \brief Waits for all the futures to be done, or for the timeout to expire.
    ///
    /// \param d Timeout duration.
    ///
    /// \return True if all the futures finished, false if the timeout was reached.
    bool wait(std::chrono::nanoseconds d) override;

    ///
    /// \brief Waits for all the futures to be done, or for the timeout to expire.
    ///
    /// \bug This currently calls wait with the specified argument on all futures, which may lead to a much longer total
    /// time.
    ///
    /// \param cycles Timeout duration.
    ///
    /// \return True if all the futures finished, false if the timeout was reached.
    bool wait(std::uint64_t cycles) override;
};

class NopFuture final : public Future {
public:
    NopFuture();
    ~NopFuture() override;
    [[nodiscard]] bool isDone() const override;
    void wait() override;
    bool wait(std::chrono::nanoseconds d) override;
};
