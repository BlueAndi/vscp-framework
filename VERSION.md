How to release a software version?
==================================

1. Update the version in vscp/doc/Doxyfile.
2. Generate a new framework documentation.
3. Update CHANGELOG.md.
4. Commit and push all changes.
5. Create a github tag and mark the version with "pre" for pre-release, e.g. "V1.0.0pre".
6. Execute coverity tasks and update the source code accordingly.
7. If all bugfixes done, create a github tag for the release, e.g. "V1.0.0".

Software version format: V<major>.<minor>.<patch>
- Increase the major number in case of incompatible interface changes or incompatible behavior. That means that the user of the framework has to adapt its implementation too.
- Increase the minor number in case of compatible interface and behavior changes. The user must not adapt its implementation, just use the new version.
- Increase the patch number in case of bugfixes only.