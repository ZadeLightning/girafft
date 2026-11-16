# Girafft project site

A responsive one-page portfolio and build log for Girafft. The main experience follows the project timeline with real boat photos and video clips tied to each milestone.

## Run locally

```bash
npm install
npm run dev
npm run build
npm run preview
```

The Vite base path is relative so the site works as a GitHub Pages project site even if the repository is renamed. GitHub Pages deployment runs through the workflow in `.github/workflows/deploy-pages.yml` after Pages is enabled with GitHub Actions in repository settings.

## Content and imagery

- The timeline has six chapters matching the folders in `images/timeline/`.
- Put candidate photos and videos in each chapter's `all/` folder. Put selected media directly in its chapter folder and give it a descriptive filename. Only the selected files appear on the site.
- Each timeline item has its own swipeable, horizontally scrollable photo/video carousel with arrow buttons. Supported file types are JPG/JPEG, PNG, WebP, GIF, MP4, WebM, MOV, and M4V; MP4/H.264 is recommended for video compatibility.
- The carousel reads selected files automatically from `images/timeline/<chapter>/*`; it ignores `images/timeline/<chapter>/all/*`. Chapters without selected media show a “Photos & video coming soon” card.
- Photos in the hero/gallery continue to use optimized copies in `public/images/`; original photos stay in `images/`.
- `boat-mockup.png` remains in the hero as a concept rendering and is labeled as such; the timeline now focuses on real project media.
- Keep completed milestones separate from the roadmap. The first MVP sea trial succeeded on November 8, 2026. Girraft remains manually controlled; do not describe it as autonomous.

## Project context

Read `../USV_Master_Context.md` before changing project facts, hardware descriptions, or status. It is the source of truth for the vessel and the user's website direction.
