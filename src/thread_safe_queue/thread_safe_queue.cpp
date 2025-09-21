#include "thread_safe_queue.hpp"

template <typename T>
void ThreadSafeQueue<T>::push(T x) noexcept {
        std::unique_lock locker(m_mtx);
        m_queue.push_back(std::move(x));
        m_sem.release();
}

template <typename T>
std::optional<T> ThreadSafeQueue<T>::pop() noexcept {
        m_sem.acquire();
        std::unique_lock locker(m_mtx);

        if(m_is_finished)
                return {};
        T ret = std::move(m_queue.front());
        m_queue.pop_front();
        if(m_queue.empty() && m_can_be_finished) {
                m_finished_sem.release();
        }
        return ret;
}

template <typename T>
void ThreadSafeQueue<T>::finish_pops(size_t n_poppers) noexcept {
        std::unique_lock locker{m_mtx};
        if(!m_queue.empty()){
                m_can_be_finished = true;
                locker.unlock();
                m_finished_sem.acquire();
                locker.lock();
        }
        m_is_finished = true;
        m_sem.release(n_poppers);
}

template class ThreadSafeQueue<int>;
