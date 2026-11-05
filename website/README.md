# ASV Website — Codex Starter Brief

## Purpose

Build a simple, polished project website that documents the user's hands-on surface-vessel build. This is a showcase and engineering build log—not a control interface for the boat.

## Start here

Before making changes, read the repository's `ASV_Master_Context.md`. Treat it as the authoritative source for project facts, hardware, progress, and terminology. Use documented details accurately; do not invent specifications, test outcomes, or claim the vessel is autonomous or water-tested unless the context supports that.

## Initial website direction

- Use **React + Vite**.
- Keep the first iteration to a single responsive scrolling page.
- Visual direction: clean, modern marine/engineering portfolio; restrained dark navy, neutral backgrounds, clear typography, strong project photography.
- Potential sections: project overview/hero, vessel and hull, propulsion, control electronics, milestone/build log (including POC 6 and POC 7), gallery, current status/next milestone, repository link.
- Use the user's actual photos, diagrams, and test results when supplied. Until then, use clearly marked placeholders rather than misleading stock photos presented as the actual boat.
- Keep content editable and components straightforward. Avoid unnecessary animation and complexity.

## Repository and scope constraints

- The website lives in this `website/` directory, alongside existing Arduino sketches and project documentation.
- Do not move, rename, reformat, or otherwise modify existing sketches or unrelated project files.
- Do not add a backend, database, CMS, telemetry, or boat-control functionality.
- Prefer a small dependency footprint and accessible, responsive HTML.
- Do not publish secrets, private credentials, or sensitive information.

## Build and local preview

Use the commands from this directory:

```bash
npm install
npm run dev
npm run build
npm run preview
```

Check the production build with `npm run build` before considering a change complete.

## GitHub Pages deployment

The repository is `ZadeLightning/girraft`; the expected project-site URL is:

`https://zadelightning.github.io/girraft/`

Configure Vite's `base` path for the repository subpath (`/girraft/`) when appropriate. Set up GitHub Actions to build and deploy the Vite `dist/` output to GitHub Pages. Do not assume Pages settings or workflow permissions are already configured; inspect and explain any required repository settings.

## Working style

1. Inspect the existing repository and this brief before editing.
2. Implement the smallest useful first version; don't overbuild.
3. Summarize files changed, design choices, and any assumptions.
4. Run and report the build result. Do not claim a command ran if it did not.
5. Update `ASV_Master_Context.md` when a meaningful website architecture, deployment, or project milestone decision is made.
