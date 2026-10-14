# KM3NeT Dataformat Changelog
## Unreleased changes
- NONE

## Version 3

### v3.6.1
- include km3neut definitions
- updated fitparameters descriptions

### v3.6.0
- Updated tree definitions for oscillation open data

### v3.5.1
- Tree definition for oscillations open data

### v3.5.0
- include cmake build pipeline
- include python test
- include csv to code converter
- include JMuonFeature Number of Hits
- include root auto flush feature
- add C++ example to read Evt files
- raise class definition of JDAQHit
- improve readme

### v3.4.0
- Update Readme and documentation
- Add check of validity of rate to DAQ summary frame
- Add get function for arbitrary DAQUTC offset

### v3.3.0
- Update fields for W2LIST_KM3BUU (25: lepton prop resample attempts)
- Update reconstrucion definitions: use scope resolution operator
- Update JDAQEvent[Header]
- Add muon features
- Update documentation

### v3.2.0 / 2023-07
- added new status flags of modules
- updated header and multi-header class
- moved struct to classes fur JDAQ*, as well change data types

### v3.1.0 / 2023-3-27
- added static member for version in offline classes
- added Multi-head join functionality

### v3.0.0 / 2022-11-07
- add a test suite
- make: add python environment support and zsh support
- change compressino level of timeslices
- improve documentation
- include re-weighting support
- include UUID for each event
- include mutliheader support
- update UTC support

## Version 2

### v2.2.0 / 2022-03-24
- added muon bundle definitions
- added `counter` member to `Trk`

### v2.1.0 / 2021-09-01
- include full path in include directives
- Added aashower fit parameters
- remove root version v5 from CI/CD


### v2.0.0 / 2021-03-09
- update of base root version to v6.22.06
- added definition of applications, root file definitions (names and numbers),
  daqdatatypes and PMT status
- added definitions for mother/daughter relation in MC files and update of
  MC header
- correction of time handling
- update of precision for ASCII data
- start of test framework
- JMUONFIT and JSHOWERFIT has been removed from the reconstruction definitions
  since they only caused confusion and had no added value.
- DAQ datatypes have been extracted to `definitions/daqdatatypes.csv`
- Updated versions of MC helpers (making them const) in `Evt.h`
- `get_hit_by_id` and `get_mc_hit_by_id` methods were removed
- added `tools/time_converter.hh` which provides helpers for time conversions
  between DAQ and MC time frames

## Version 1

### v1.3.1 / 2020-11-09
- Fixed MC event time init

### v1.3.0 / 2020-11-09
- New MC time handling

### v1.2.4 / 2020-09-25
- fixed typo in Dusj stages: DUSJSHOWERBEND -> DUSJSHOWEREND

### v1.2.3 / 2020-06-28
- update of rootcint compiler flag
- update of root v6 version and disabling of root v5

### v1.2.2 / 2020-06-17
- update of primary/neutrino id handling 

### v1.2.0 / 2020-05-18
- adapt fit stages
- include reconstruction get/has functions
- update w2 list 

### v1.1.2 / 2020-01-23
- Add tests for the generated definition files
- Fix Julia and Python structures

### v1.1.1 / 2020-01-23
- Minor fix to the CI, tagging and changelog

### v1.1.0 / 2020-01-23
- Fixed the CI to only trigger on tags
- Python data structures are changed to dictionaries in ``definitions``
- Removed primarly lepton tag in offline format
- Added ``JPP_RECONSTRUCTION_TYPE``

### v1.0.1 / 2019-12-16
- Added CI to notify projects which depend on this library when new releases
  are made (via annotated Git tags)

### v1.0.0 / 2019-12-16

- First release of the newly extracted data format library from Jpp and aanet
