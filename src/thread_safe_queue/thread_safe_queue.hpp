#ifndef THREAD_SAFE_QUEUE_198312098309128093809283091
#define THREAD_SAFE_QUEUE_198312098309128093809283091

#include <deque>
#include <mutex>
#include <optional>
#include <semaphore>

template <typename T>
class ThreadSafeQueue {
public:

        ThreadSafeQueue()  noexcept = default;
        ~ThreadSafeQueue() noexcept = default;

        void push(T x) noexcept;
        std::optional<T> pop() noexcept;

        void finish_pops(size_t n_poppers) noexcept;

private:
        std::deque<T> m_queue = {};

        std::mutex m_mtx{};
        bool m_is_finished = false;
        bool m_can_be_finished = false;
        std::counting_semaphore<1<<15> m_sem{0};
        std::binary_semaphore          m_finished_sem{0};
};
#endif//THREAD_SAFE_QUEUE_198312098309128093809283091
