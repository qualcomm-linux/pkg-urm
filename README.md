# pkg-template

This repository serves as a template for creating Debian package repositories within the Qualcomm Linux ecosystem. It provides the essential structure, GitHub workflows, and configuration necessary to integrate with the [qcom-build-utils](https://github.com/qualcomm-linux/qcom-build-utils) repository, enabling standardized Debian package building processes.

## Quick Start

To create a new Debian package repository using this template:

1. Navigate to this repository's GitHub page and click the **"Use this template"** button located in the top right corner.
2. Select **qualcomm-linux** as the organization in the drop-down menu. This is necessary.
3. Name the new repository with the prefix `pkg-` to adhere to the naming convention for package repositories. This is necessary.
4. Ensure the **"Include all branches"** option is enabled. Otherwise by default, only the default branch "main" is cloned.

## Branches

- **main**: The primary branch containing workflow logic in the `.github/` folder, along with boilerplate documentation files such as license, contribution guidelines, and this README.
- **debian/qcom-next**: An orphan branch with unrelated history from main. It contains a debian/ folder with template files. Its just to give a starting point and structure. The first job for the user templating from this repo will be to update this debian/ folder. The information about the name **debian/qcom-next** and other naming conventions can be found [here](https://qualcomm-confluence.atlassian.net/wiki/spaces/LinuxCoreOS/pages/2879858691/pkg-+repository+specification)

## Workflows

The `main` branch includes the following workflows in the `.github/workflows/` directory:

- **qcom-preflight-checks.yml**: A sanity check workflow inherited from the base Qualcomm template.
- **stale-issues.yml**: A workflow for managing stale issues, also inherited from the base template.
- **build-debian-package.yml**: Builds the Debian package for this repository. This workflow serves as an entry point that invokes reusable workflows from the centralized qcom-build-utils repository.
- **pr-pre-post-merge.yml**: This workflow executes during a PR, and once the PR is merged. 
- **promote-upstream.yml**: Promotes the package's tracking version to a new upstream release. This workflow also triggers reusable workflows in qcom-build-utils.
- **release**: Used to trigger a release of a package

## IMPORTANT: Workflow to paste in the upstream source repo

The .github/TO_PASTE_IN_UPSTREAM_REPO/pkg-build-pr-check.yml needs to be transfered over to the source repo.
Once this is done, delete the TO_PASTE_IN_UPSTREAM_REPO as it is not necessary to keep in this repository once the workflow has been transfered.

**This workflow needs to be put in the default branch of the source repo (likely branch main unless you modify it), otherwise it wont work** This is per new December Github update

## Repository Configuration

### Runners

All the workflows are running on the github arm64 runners. Some sections need to run on the AWS runners where access to S3 buckets and artifactory. You will need to ask **Steve Manley** to enable the repo for the AWS runners.

### GHCR Registry Access

The build workflows rely on the `pkg-builder` container image hosted in the qualcomm-linux GitHub Container Registry (GHCR). For your newly created repository to be able to pull this image, it must be explicitly granted `packages:read` access.

To request this, please contact **Mark Matyas** (mmatyas@qti.qualcomm.com) and ask him to add your repository to the list of repositories authorized to access the `pkg-builder` package. The relevant settings page is:

https://github.com/orgs/qualcomm-linux/packages/container/pkg-builder/settings

### Repository Variables

Set the following repository variables to establish links between upstream and package repositories:

- **UPSTREAM_REPO_GITHUB_NAME**: In this package repo, this is the GitHub name of the upstream repository (e.g. in the case of the pkg-example, `qualcomm-linux/qcom-example-package-source`).
- **PKG_REPO_GITHUB_NAME**: This variable is set in the upstream project repo; the GitHub name of the package repository (e.g., `qualcomm-linux/pkg-example`).

### Branch Protection Rules

Configure branch protection for `debian/**` and `qcom/**`:

- Restrict deletions.
- Require pull requests before merging.
- Block force pushes.
- Add `build / build-debian-package` as a required status check.
- Add the `qcom-service-bot` account with admin rights
- Add the Admin role to the branche protection ruleset so that the qcom-service-bot can push directly to those branches
### Additional Settings

- Enable **"Automatically delete head branches"** for pull requests.
- Allow only merge commits for pull request merges.
- Enable **release immutability** in the upstream repository.
- Add the Qualcomm Github Service bot as a user with write access :
  - While the repo is private, add the Github user **qcom-service-bot**.
  - If/when the repo is made public, there will be a big change in how the contributors are handled. 
    After that, the contributors list is cleared, and one need to re-enroll as a contributor. The way
    to do that is completely different from when the repo was private. When it was private, the creator
    of the repo had the possibility to go into the repo settings and add whoever. When made public, the
    repo's maintainer/contributor list is managed by a Qualcomm internal mailing list. If the repo is
    named pkg-foo, then the Maintainer list will be named Maintainers.pkg-foo, and one need to request
    access via https://lists.qualcomm.com, find the list and ask access. For the bot, you must add it via
    its qualcomm username **githubservice**@qti.qualcomm.com, as opposed to its Github handle above.

## Making your pkg-repo to public

When your pkg-repo is mature enough, it will need to be made public.

### How to make the repo go public
 
The first step in making a repo go public is the Tradesmark/legal process.
Your POC (Person of Contact) in this step is [Stephanie Arce](sarce@qti.qualcomm.com)

You need to [complete an OSS Contribution Request](https://jira-dc4.qualcomm.com/jira/secure/CreateIssue.jspa?pid=46536&issuetype=13440)

Here is a bit of help on the mandatory fields when filling the form:

- Components : Select "Other"
- Summary : Give a short description such as "Debian packaging repository for X"
- Project name: This would be the repo name, such as "pkg-example"
- Destination URL: Give the Github URL, such as "www.github.com/qualcomm-linux/pkg-example"
- Description: Give a description about the repo being a debian packaging repo, which packages another upstream project X
- Project Licenses: Give the license of the repo. Unless something changes, it should remain  BSD-3-Clause License.
- Contribution plan: 
  - Project Description: You can give the same project description as above
  - What are ourr general plan with the Project: Talk about it being the debian packaging for project X
  - Do we have any Leadership positions in the project: Give yourself/manager
  - Is Qualcomm expanding the project by contributing technology implementations: Likely a simple no

More information can be found [here](https://github.qualcomm.com/pages/osdo/handbook/qcom-github/docs/new-project-checklist/)

Once submitted, it will generally take a couple of days to a week to get approval from Legal.

The, once you receive an email about the acceptance of the Legal team, you will have to submit a ticket about enabling the repo.

In this process, your POC will be [Mark Matyas](mmatyas@qti.qualcomm.com)

Complete the form : https://ossops.qualcomm.com/github/enable-repo/

### Note about Github Repo roles after going public

When creating a repo withing the organization, it always starts as a private repo and one need to go through an approval
process in order to have the repo go public. 

Upon creation of the repo, the person who created it gets to be assigned the maintainer responsability and has full control
over it in the settings. When the repo goes public, this changes. The repo creator stops having all the powers over the repo,
and in order to join the repo as a contributor, one needs to enroll via a github mailing list instead of directly through the repo
settings. 

This is the link to go to in order to ask to be a contributor or a maintainer of a Qualcomm publib github repo : 
https://lists.qualcomm.com/ListManager

In the searchbox, one need to find the contributor/maintainer list corresponding to the repo. 
If the repo is named "pkg-example", then the list(s) to search for are : 

- contributors.pkg-example
- maintainers.pkg-example

It takes about an hour between the list acceptance and the new role to reflect in Github

## Getting in Contact

For support or inquiries, contact sbeaudoi@qti.qualcomm.com.

## License

pkg-template is licensed under the [BSD-3-Clause License](https://spdx.org/licenses/BSD-3-Clause.html). See [LICENSE.txt](LICENSE.txt) for the full license text.
