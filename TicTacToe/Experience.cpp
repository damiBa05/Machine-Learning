#include "Experience.hpp"

ReplayBuffer::ReplayBuffer(int cap) : capacity(cap){}

void ReplayBuffer::addExperience(const Experience& experience){
    if (buffer.size() >= capacity) {
        buffer.erase(buffer.begin()); // Remove the oldest experience
    }
    buffer.push_back(experience);
}

std::vector<Experience> ReplayBuffer::sample(int batchSize) const {
    std::vector<Experience> batch;
    int actualBatchSize = std::min(batchSize, static_cast<int>(buffer.size()));
    std::vector<bool> alreadyPicked(buffer.size(), false);

    for (int i = 0; i < actualBatchSize; ++i) {
        int randomIndex = rand() % buffer.size();
        if (!alreadyPicked[randomIndex]) {
            alreadyPicked[randomIndex] = true;
            batch.push_back(buffer[randomIndex]);
        } else {
            --i; // retry - now guaranteed to eventually succeed
        }
    }
    return batch;
}

int ReplayBuffer::size()const{
    int size = buffer.size();
    return size;
}