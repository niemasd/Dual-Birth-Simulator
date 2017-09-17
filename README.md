# Dual-Birth Simulator
The algorithm to sample trees under the Dual-Birth model as described in *A two-state model of tree evolution and its applications to* Alu *retrotransposition* is O(*n* log *n*). This repository contains a O(*n*) algorithm to sample trees under the Dual-Birth model that has been implemented efficiently in C++ (whereas the original implementation was in Python), resulting in significantly faster simulations.

## Usage ##
Simply clone the repository and compile using the `make` command. Usage is as follows:

`dualbirth lambda_A lambda_B [-n end_taxa] [-t end_time] [-r num_replicates]`

* You must specify a stopping criterion using either `-n` or `-t` (or both, if desired)
* The default value of `num_replicates` is 1

## The Original Algorithm ##
In the original Dual-Birth sampling algorithm, we would start with a single leaf with time 0 (the root), and while the termination condition was not yet reached, we would iteratively pop the leaf with the lowest time *t*, create two new leaves to be its children (one with time *t* plus a time sampled from an exponential random variable with rate *λ_a* and another with time *t* + a time sampled from an exponential random variable with rate *λ_b*). Each iteration of the loop was constant-time, but maintaining a priority queue of *n* elements made the algorithm O(*n* log *n*) overall.

## The Linear-Time Algorithm ##
In this algorithm, we similarly start with a single leaf (the root) and iterate until the termination condition has been reached. However, in each iteration, we do the following:

1. Sample the time of the next splitting event by sampling an exponential random variable with rate (*n_i*)(*λ_a*)+(*n_a*)(*λ_b*), where *n_i* is the current number of inactive leaves and *n_a* is the current number of active leaves
    * Each leaf is a Poisson process, so the time until a given leaf will undergo a splitting event is given by an exponential random variable its rate (*λ_b* for active leaves and *λ_a* for inactive leaves)
    * Therefore, the time until the next branching event overall is the minimum of each of those exponential random variables, which is equivalent to an exponential random variable whose rate is the sum of all of their rates
    * There are *n_a* active leaves and *n_i* inactive leaves, so the overall sum of their rates is simply (*n_i*)(*λ_a*)+(*n_a*)(*λ_b*)
2. Choose the branch to split by first sampling a Bernoulli random variable with probability (*n_a*)(*λ_b*)/((*n_i*)(*λ_a*)+(*n_a*)(*λ_b*)): if successful, randomly pick an active leaf; if unsuccessful, randomly pick an inactive leaf
    * For a collection of exponential random variables, the probability that any given one will be the minimum is its rate divided by the sum of all the rates. The possible outcomes for which specific exponential random variable was the minimum are mutually exclusive, so the probability that the minimum happened to be an active leaf is simply the sum of all active leaves' rates divided by the sum of all leaves' rates, which is simply (*n_a*)(*λ_b*)/((*n_i*)(*λ_a*)+(*n_a*)(*λ_b*)) because there are *n_a* active leaves, each with rate *λ_b*
    * Given that the minimum was an active leaf (i.e., the Bernoulli trial was successful), all active leaves are equiprobable of being the minimum, so we randomly pick an active leaf. Similarly, if we are given that the minimum was an inactive leaf (i.e., the Bernoulli trial was unsuccessful), all inactive leaves are equiprobable of being the minimum, so we randomly pick an inactive leaf
3. Remove the chosen leaf, create two new leaves (its children), and add them to their respective sets (*active* and *inactive*)

Like the original algorithm, we iterate exactly *n* times, but instead of having to maintain a sorted set of leaves, we simply perform constant-time operations at each step: we sample an exponential random variable with a rate that is computed in constant time, then sample a Bernoulli random variable with a rate that is computed in constant time, randomly remove an element from a set in constant time, create two new nodes in constant time, and add them to their respective sets in constant time.