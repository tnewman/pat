# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.6.1] - 2021-05-16
- Upgraded dependencies

## [0.6.0] - 2020-11-29
### Added
- Added documentation comments to `rustpat`

## [0.5.1] - 2020-11-08
- Fixed memory leak in audio decoder
- Run CI jobs on Ubuntu 18.04 and Ubuntu Latest

## [0.5.0] - 2020-11-07
- Added `rustpat` Rust wrapper
- **[BREAKING CHANGE]** Replaced `pat_open` with `pat_init` in `libpat`
- **[BREAKING CHANGE]** Removed `PAT` struct from `libpat`

## [0.4.1] - 2020-10-28
### Changed
- Updated dependencies
- Updated Github Actions CI/CD Windows FFmpeg installation
- Updated Github Actions CI/CD Windows environment variables

## [0.4.0] - 2020-05-24
### Added
- Official support for `pypy`, including requiring `pypy` tests to pass in CI
- Added default export to `nodepat`
- Added TSDocs to `nodepat`

## Changed
- Updated `nodepat` example to use TypeScript
- Updated `nodepat` and `pypat` dependencies

## [0.3.0] - 2020-04-03
### Added
- GitHub Action Continuous Integration/Continuous Deployment

### Changed
- Switch to `node-gyp`
- Switch to native Python C extension build
