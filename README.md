# PA_SSD


v0.6

v0.6 is the current stable version, used for pre-evaluation of graduation
paper for Master's Degree.  

Features and Characteristics:

- Write-only (read not implemented)
- Address must be aligned to page
- Only page-length (or multiples of) requests are accepted
- Direct NAND writes are implemented
- Proposed algorithm can be turned on/off or tuned, by modifying software_cache
- DRAM queueing is disabled
- GC is not yet tested

To be added in the future :

- Add data confirmation
- Implement bitmap in hardware (it's done in software)
- clean up headers, so that all parameters can be seen in one file
- implement read
- test GC/ implement GC correctly



