#include "robin/engine/cdf.h"
#include "robin/generation/transformation.h"

#include <queue>
#include <vector>

// Implement Walker Alias Method (one time construction for O(1) lookups)
AliasTable generateDistribution(const std::vector<Transformation>& transforms) {
    std::size_t count{ transforms.size() };
    AliasTable table;
    table.probabilities.resize(count, 1.0f);
    table.aliases.resize(count, 0);

    if (count == 0) {
        return table;
    }

    float totalWeight{};
    for (const auto& transform : transforms) {
        totalWeight += transform.weight_;
    }

    std::vector<float> scaled_probs(count);
    std::queue<std::size_t> small{};
    std::queue<std::size_t> large{};

    for (std::size_t i{ 0 }; i < count; ++i) {
        scaled_probs[i] = (transforms[i].weight_ / totalWeight) * count;
        if (scaled_probs[i] < 1.0f) {
            small.push(i);
        }
        else {
            large.push(i);
        }
    }

    while (!small.empty() && !large.empty()) {
        std::size_t l{ small.front() }; 
        small.pop();

        std::size_t g{ large.front() };
        large.pop();

        table.probabilities[l] = scaled_probs[l];
        table.aliases[l] = g;

        scaled_probs[g] = scaled_probs[g] - (1.0f - scaled_probs[l]);

        if (scaled_probs[g] < 1.0f) {
            small.push(g);
        }
        else {
            large.push(g);
        }
    }

    while (!large.empty()) {
        table.probabilities[large.front()] = 1.0f;
        large.pop();
    }
    while (!small.empty()) {
        table.probabilities[small.front()] = 1.0f;
        small.pop();
    }

    return table;
}

std::size_t findIndex(const AliasTable& table, float uniformZeroToOne, std::size_t uniformIndex) {
    if (uniformZeroToOne < table.probabilities[uniformIndex]) {
        return uniformIndex;
    }
    else {
        return table.aliases[uniformIndex];
    }
}
