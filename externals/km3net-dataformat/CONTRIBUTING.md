#Contributing to km3net-dataformat

Thanks for your interest in contributing! We welcome bug reports, feature requests, documentation improvements, and code contributions.

There are many ways you can help:

- **Report Bugs**: Use [GitLab Issues](https://git.km3net.de/common/km3net-dataformat/-/issues/). Please include steps to reproduce (including input data if needed), expected vs. actual behavior.
- **Request Features**: Open an issue with `[Feature Request]` in the title.
- **Improve Documentation**: Fix typos, improve clarity, or add missing details.
- **Submit Code**: See the workflow below.

## 🏁 Getting Started

1. Clone the repository: `git clone https://git.km3net.de/common/km3net-dataformat`
2. [Install](/README.md#installation-from-source


## 🌱 Branching & Workflow

- To contribute code you create branches off `master`.
- Use descriptive branch names. When the branch addresses an [existing issue](https://git.km3net.de/common/km3net-dataformat/-/issues), please prefix the branch name by the issue number.
- Keep merge requests small and focused.

> [!important]
> When adding/removing source code files, you _must_ update the relevant `CMakeLists.txt` file(s).


## 📝 Commit messages

We do not currently have a convention for commit messages, except to mark commits that should be included in the next release (see below the Releases section), which should start with "Changelog:".

Still, you may want to read e.g. [Conventional Commits](https://www.conventionalcommits.org/) for inspiration.


## 🔄 Merge Request Process

1. Make sure your branch is up to date with `master`.
2. Run tests locally (see below)
3. Ensure documentation is updated if needed.
4. Submit a MR with a clear title and description.
5. (optional) Mark the MR as draft if the MR is not yet ready to be reviewed and/or merged (but you'd like to share it to discuss it, for instance).
6. A maintainer will review your changes.

The merge request must be approved by at least one of the maintainers before it
can be merged.

> We expect maintainers themselves to follow this rule, i.e. wait for another
> pair of eyes to go through the proposed code before actually hitting the
> merge button.


## 🪄 Coding/Formatting Standards

We do not (yet) have coding standards. We do discuss about [formatting standard](https://git.km3net.de/common/jpp/-/merge_requests/564) though.


## 🧪 Testing

Add tests under `tests/`.
Do not forget to update the `tests/CMakeLists.txt` file accordingly.
Please add or update tests for any new functionality or bugfix.
Run the tests using `ctest` (from the build directory).

## 📚 Documentation

- see [README.md](README.md#documentation-1)
- Update docs whenever you add new features or change behavior.


## 📦 Releases

- We use **[Semantic Versioning](https://semver.org/)**.

Note that once a tag is made, the CI automatically creates issues into dependent projects.

## 💬 Questions?

- Join the [Computing and Software Open Hour Meeting](https://indico.cern.ch/category/4743/), each Monday at 9:00 CEST.
- Open a discussion in the project's [gitlab issues](https://git.km3net.de/common/km3net-dataformat/-/issues/?sort=closed_at_desc&state=opened&first_page_size=100)
- Or ask in the [software chat](https://chat.km3net.de/channel/software)

