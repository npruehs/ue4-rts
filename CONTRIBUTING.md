# Contributing

You'd like to help make this plugin even more awesome? Seems like today's our lucky day! In order to maintain stability of the tool and its code base, please adhere to the following steps, and we'll be pleased to include your additions in our next release.

Note that this plugin is distributed under the [MIT License](https://github.com/npruehs/ue4-rts/blob/develop/LICENSE). So will be your code.

## How to contribute

### Step 1: Choose what to do

If you've got no idea how to help, head over to our [issue tracker](https://github.com/npruehs/ue4-rts/issues) and see what you'd like to do most. You can basically pick anything you want to, as long as it's not already assigned to anyone.

If you know exactly what you're missing, [open a new issue](https://github.com/npruehs/ue4-rts/issues/new) to begin a short discussion about your idea and how it fits the project. If we all agree, you're good to go!

### Step 2: Fork the project and check out the code

Real-Time Strategy Plugin for Unreal Engine 4 is developed using the [GitFlow branching model](http://nvie.com/posts/a-successful-git-branching-model/). In order to contribute, you should check out the latest `develop` branch, and create a new feature or hotfix branch to be merged back.

### Step 3: Implement your feature or bugfix

Clearly, everybody's got their own approach here. However, we'd still like you to keep a few things in mind, to ensure the stability and consistency of the plugin for everyone:

* We're using the official [Coding Standard](https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/index.html) provided by Epic Games.
* When you're adding support for a newer engine version, make sure that you don't break support for previously supported engine versions. We must not force our users to upgrade their engine just because of updating the plugin.

### Step 4: Open a pull request

Finally, [open a pull request](https://help.github.com/articles/creating-a-pull-request/) so we can review your changes together, and finally integrate it into the next release.


## Release Checklist

Internally, we're using the following checklist when preparing for a new release:

* Check pending pull requests
* Create release branch
* Add examples for new features where appropriate
* Run all automated tests
* Update documentation (README, images, spelling, table of contents)
* Increase version number (and engine version, if necessary)
* Create plugin package (and remove intermediates)
* Check plugin package in another project
* Merge release branch with tag
* Add a new GitHub release with release notes (including deprecation notes)
* Update GitHub issues and milestones
* Notify community (e.g. forums)
