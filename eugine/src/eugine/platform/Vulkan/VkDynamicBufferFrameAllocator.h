//
// Created by aiden on 7/23/22.
//

#ifndef EUGINE_VKDYNAMICBUFFERFRAMEALLOCATOR_H
#define EUGINE_VKDYNAMICBUFFERFRAMEALLOCATOR_H

#include <list>

namespace eg::rendering::VKWrapper {
    template <typename T>
    class VkDynamicBufferFrameAllocator {
    public:
        typedef Ref<T> (*BufferCreateFN)(void* udata);

        VkDynamicBufferFrameAllocator<T>(BufferCreateFN bufferCreateFn, void* udata) : m_bufferCreateFN(bufferCreateFn), m_udata(udata) {};
        ~VkDynamicBufferFrameAllocator(){
            freeAllocations();
        }

        void freeAllocations() {
            for(auto& allocation : m_buffers) {
                allocation.second = false;
                allocation.first = Ref<T>(nullptr);
            }
            m_buffers.clear();

            m_currentUnallocatedBuffer = m_buffers.begin();
        }

        void resetAllocations() {
            m_currentUnallocatedBuffer = m_buffers.begin();
        }

        Ref<T> allocate() {
            if(m_currentUnallocatedBuffer == m_buffers.end()) {
                m_buffers.template emplace_back(std::make_pair(m_bufferCreateFN(m_udata), true));
                return (m_buffers.back().first);
            }

            auto it = m_currentUnallocatedBuffer;
            m_currentUnallocatedBuffer++;

            if(it->second == false) {
                it->first = m_bufferCreateFN(m_udata);
                it->second = true;
                return it->first;
            } else {
                return it->first;
            }
        }

    private:
        BufferCreateFN m_bufferCreateFN;
        void* m_udata;

        std::list<std::pair<Ref<T>, bool>> m_buffers;
        typename std::list<std::pair<Ref<T>, bool>>::iterator m_currentUnallocatedBuffer = m_buffers.begin();
    };
}

#endif //EUGINE_VKDYNAMICBUFFERFRAMEALLOCATOR_H
