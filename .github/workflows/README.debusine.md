The `debusine-*.yml` files form a set of workflows that have been imported from
the
https://github.com/qualcomm-linux/qli-ci/tree/main/pkg-workflows/debusine
reference location to enable Debusine CI. They must not be changed except to
copy updates from the reference location. They will be updated as the reference
location is updated. See
[README.md](https://github.com/qualcomm-linux/qli-ci/blob/main/pkg-workflows/debusine/README.md)
for details.

To temporarily disable Debusine workflows for this repository without removing
workflow files or environments, set the repository Actions variable
`DEBUSINE_WORKFLOWS_DISABLED` to `true`. Delete the variable, or set it to any
other value, to re-enable the workflows.
