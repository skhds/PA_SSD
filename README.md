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


v0.71

Added :


- Data compare feature, to verify if the resulting memory storage matches the logical memory map acknowledged from the host 
- Prints in cout for each false entry
- Not completely verified to be accurate yet, but I'm seeing some errors (null data stored)


To do :

- Clean up headers, I'll try to make a "configure" file for setting up parameters 
- Better format for error detections
- Edit and fix whatever caused data errors
- More lightweight simulation... only one char for each 512B sector



v0.72

Done :


- Headers are re-organized, now you only have to look at configurables.h (hopefully) for editing simulation parameters
- Modules are also re-organized
- RAM latency modeling is corrected.. no concurrent RAM accesses are allowed now.

Issues :

- Different values for throughput, possible error in BW or latency modeling
- Data compare is disabled for now

To do :

- Test and fix bandwidth-related things
- Another metric for RAM lifetime (maybe "max counts" or "average counts of top 1%")
- Implement start-gap
- Data compare for later
