# Scalable Data Store (Online Client-Server): Enforcing Quotas and Managing Resources

This project aims to implement some standard resource management concepts, such as quotas and most-recently-used (MRU) tracking. It offers a unique and efficient solution for managing resources in online services, ensuring they are not abused and used fairly and efficiently.

## Introduction

The core idea of this project revolves around an online service. The system tracks the MRU items, enforces quotas on upload bandwidth, download bandwidth, and number of requests, ensuring optimal performance and fairness.

## Project Overview

This system includes the following major components:

1. **MRU Tracking**: A `KVT` (TOP) command is added to the client and server. `KVT` is similar to `KVA`, except that the server will have a configurable threshold `T`, and track the `T` most recently accessed keys. `KVT` will return as many of these keys as have not been deleted. 

2. **Quotas Management**: Three quotas are enforced: download bandwidth, upload bandwidth, and number of requests. If a user requests more data (via `KVG`, `KVA`, and `KVT`) in a time interval than the threshold allows, then subsequent `KVG`, `KVA`, and `KVT` commands within the interval will result in `ERR_QUOTA_DOWN`. Similar rules apply to upload quotas and request-per-interval quota.

## How to Run

To run the project, navigate to the `p4` folder in your terminal and follow the steps:

1. Start a container
2. Run `chmod +x solutions/*.exe`
3. Compile the project using the provided `Makefile`
4. Start the server using the generated executable
