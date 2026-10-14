import gitlab
import os

origin = "https://git.km3net.de"
# Private token is set as an env var
private_token = os.getenv("PRIVATE_TOKEN")
gl = gitlab.Gitlab(origin, private_token, api_version='4')
gl.auth()

projects_to_notificate = [
    451,  # km3io
    223,  # Jpp
    184,  # aanet
    470,  # KM3io.jl
    679,  # KM3NeT TestData
    292,  # km3sim
    314,  # gSeaGen
]

for project_id in projects_to_notificate:
    project = gl.projects.get(project_id, lazy=True)

    issue = project.issues.create({
        'title':
        'New KM3NeT Dataformat Release {}'.format(os.getenv("CI_COMMIT_TAG")),
        'description':
        "There is a new version of the KM3NeT Dataformat available, "
        "please update!"
        "\n\nhttps://git.km3net.de/common/km3net-dataformat/-/tags",
    })
    issue.labels = ['io', 'todo']
    issue.save()
