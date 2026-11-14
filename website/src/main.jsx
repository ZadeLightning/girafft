import React, { useEffect, useRef, useState } from 'react'
import { createRoot } from 'react-dom/client'
import './styles.css'

const base = import.meta.env.BASE_URL
const headerPhoto = Object.values(import.meta.glob('../images/header_photo/*.{jpg,jpeg,png,webp}', {
  eager: true,
  query: '?url',
  import: 'default',
}))[0]
const headerVideo = Object.values(import.meta.glob('../images/header_video/*.{mp4,webm,mov,m4v}', {
  eager: true,
  query: '?url',
  import: 'default',
}))[0]

const chapters = [
  {
    number: '01',
    id: 'hull',
    mediaFolder: '01 bare hull',
    label: 'Bare hull',
    title: 'A hull with a past life.',
    copy: 'Found on Facebook Marketplace, this barebones hull had been built for a SpaceX proof of concept.',
    specs: [
      <>5′8″ long · 20″ wide</>,
      <>Vinylester resin and Divinycell foam core hull, with an aluminum mast and plywood deck</>,
      <>Designed by world-renowned naval architects Morrelli &amp; Melvin</>,
    ],
    tag: '5′8″ · 20″ · Morrelli & Melvin',
    status: 'Foundation',
  },
  {
    number: '02',
    id: 'aft-hatch',
    mediaFolder: '02 aft deck hatch',
    label: 'Aft deck hatch',
    title: 'Access at the stern.',
    copy: 'Added an aft deck hatch to enable propeller mounting and stern compartment access.',
    tag: 'Hatch · Propeller mount · Stern access',
    status: 'Added',
  },
  {
    number: '03',
    id: 'propeller',
    mediaFolder: '03 propeller',
    label: 'Propeller',
    title: 'Build a mount. Make it move.',
    copy: 'Shaped and drilled an aluminum mount for the U10 thruster, attached it at the stern, and completed a successful bench test.',
    tag: 'U10 propulsion · ESC · Bench test',
    status: 'Installed',
  },
  {
    number: '04',
    id: 'steering',
    mediaFolder: '04 steering',
    label: 'Steering',
    title: 'Steer the ship.',
    copy: 'The actuator, linkage geometry, and rudder are integrated to enable steering.',
    tag: 'PA-14P actuator · Rudder linkage',
    status: 'Installed',
  },
  {
    number: '05',
    id: 'mvp-final-assembly',
    mediaFolder: '05 mvp final assembly',
    label: 'MVP final assembly',
    title: 'Finalize the MVP.',
    copy: 'Replace test setups with final wiring. Solder connectors, and encase electronics in water-safe housing. Write final MVP Arduino sketch combining throttle and steering, in addition to failsafes in the case of controller connection loss. Test full system disconnected from the computer and connected to final onboard power supply.',
    tag: 'Electrical · Arduino · Integration',
    status: 'Completed',
  },
  {
    number: '06',
    id: 'mvp-sea-trial',
    mediaFolder: '06 mvp sea trial',
    label: 'MVP sea trial',
    title: <>Sail the <em>high seas</em>.</>,
    copy: "The first sea trial took place at the San Diego Yacht Club on November 8th, 2026. The USV's performance exceeded expectations, and it remains to be seen whether it was the giraffe head or the vessel itself that caught the eye of the fellow club members that stuck around to watch it cruise between the docks.",
    tag: 'First on-water test · Successful',
    status: 'Success',
  },
]

const timelineAssetUrls = import.meta.glob('../images/timeline/*/*.{jpg,jpeg,png,webp,gif,mp4,webm,mov,m4v}', {
  eager: true,
  query: '?url',
  import: 'default',
})

const builderMedia = Object.entries(import.meta.glob('../images/builder/*.{jpg,jpeg,png,webp,mp4,webm,mov,m4v}', {
  eager: true,
  query: '?url',
  import: 'default',
})).sort(([a], [b]) => a.localeCompare(b, undefined, { numeric: true, sensitivity: 'base' }))
  .map(([path, src]) => {
    const filename = path.split('/').pop()
    const extension = filename.split('.').pop().toLowerCase()
    const caption = filename.replace(/\.[^.]+$/, '').replace(/[-_]+/g, ' ')
    return { src, filename, caption, type: ['mp4', 'webm', 'mov', 'm4v'].includes(extension) ? 'video' : 'image', mimeType: extension === 'webm' ? 'video/webm' : 'video/mp4' }
  })

const timelineMediaByFolder = Object.entries(timelineAssetUrls).reduce((mediaByFolder, [path, src]) => {
  const [, folder, filename] = path.match(/\/timeline\/([^/]+)\/([^/]+)$/) || []
  if (!folder || !filename) return mediaByFolder

  const extension = filename.split('.').pop().toLowerCase()
  const type = ['mp4', 'webm', 'mov', 'm4v'].includes(extension) ? 'video' : 'image'
  const caption = filename.replace(/\.[^.]+$/, '').replace(/[-_]+/g, ' ')
  const mimeType = extension === 'webm' ? 'video/webm' : 'video/mp4'
  const media = { type, src, filename, caption, mimeType, alt: `${caption} — ${folder}` }
  mediaByFolder[folder] = [...(mediaByFolder[folder] || []), media]
  return mediaByFolder
}, {})

Object.values(timelineMediaByFolder).forEach((media) => media.sort((a, b) => a.filename.localeCompare(b.filename, undefined, { numeric: true, sensitivity: 'base' })))

function TimelineCarousel({ chapter, isActive }) {
  const items = timelineMediaByFolder[chapter.mediaFolder] || []
  const trackRef = useRef(null)
  const videoRefs = useRef([])
  const [currentItem, setCurrentItem] = useState(0)

  useEffect(() => {
    videoRefs.current.forEach((video, index) => {
      if (!video) return
      video.muted = true
      video.volume = 0
      if (isActive && index === currentItem) {
        video.play().catch(() => {})
      } else {
        video.pause()
      }
    })
  }, [currentItem, isActive, items])

  const moveToItem = (index) => {
    if (!items.length) return
    const next = (index + items.length) % items.length
    setCurrentItem(next)
    const track = trackRef.current
    const item = track?.children[next]
    if (track && item) track.scrollTo({ left: item.offsetLeft - track.offsetLeft, behavior: 'smooth' })
  }

  return (
    <div className="chapter-media">
      {items.length ? (
        <>
          <div className="carousel-heading">
            <span>{String(currentItem + 1).padStart(2, '0')} / {String(items.length).padStart(2, '0')} <i>{items.length === 1 ? 'MEDIA ITEM' : 'MEDIA ITEMS'}</i></span>
            {items.length > 1 && <div className="carousel-arrows">
              <button type="button" onClick={() => moveToItem(currentItem - 1)} aria-label={`Previous ${chapter.label} media`}>←</button>
              <button type="button" onClick={() => moveToItem(currentItem + 1)} aria-label={`Next ${chapter.label} media`}>→</button>
            </div>}
          </div>
          <div
            className="carousel-track"
            ref={trackRef}
            onScroll={() => {
              const track = trackRef.current
              if (!track) return
              const nearest = [...track.children].reduce((best, item, index) => {
                const distance = Math.abs((item.offsetLeft - track.offsetLeft) - track.scrollLeft)
                return distance < best.distance ? { index, distance } : best
              }, { index: 0, distance: Infinity })
              setCurrentItem(nearest.index)
            }}
            role="region"
            aria-roledescription="carousel"
            aria-label={`${chapter.label} photos and videos`}
          >
            {items.map((item, index) => (
              <figure className="carousel-item" key={item.filename} aria-roledescription="slide">
                <div className="carousel-frame">
                  {item.type === 'video' ? (
                    <video ref={(node) => { videoRefs.current[index] = node }} muted loop playsInline preload="metadata" aria-label={item.caption}>
                      <source src={item.src} type={item.mimeType} />
                      Your browser does not support embedded video.
                    </video>
                  ) : <img src={item.src} alt={item.alt} loading="lazy" />}
                </div>
                <figcaption><span>{item.type === 'video' ? 'VIDEO' : 'PHOTO'}</span><b>{item.caption}</b></figcaption>
              </figure>
            ))}
          </div>
          {items.length > 1 && <div className="carousel-dots" aria-label="Choose a media item">
            {items.map((item, index) => <button key={item.filename} type="button" className={currentItem === index ? 'is-active' : ''} onClick={() => moveToItem(index)} aria-label={`Show ${item.caption}`} aria-pressed={currentItem === index} />)}
          </div>}
        </>
      ) : (
        <div className="media-empty"><span className="media-empty-mark">✳</span><b>Photos &amp; video coming soon</b><span>Build footage for this milestone will appear here as the project progresses.</span></div>
      )}
    </div>
  )
}

function Header() {
  return (
    <header className="site-header">
      <a className="wordmark" href="#top" aria-label="Girafft, back to top">
        <span className="wordmark-mark"><span /></span>
        <span>GIRAFFT<small>FIELD NOTES / 01</small></span>
      </a>
      <nav aria-label="Main navigation">
        <a href="#build">The Build</a>
        <a href="#next">Status</a>
        <a href="#future">The Future</a>
        <a href="#builder">The Builder</a>
        <a className="nav-contact" href="mailto:justin@bartell.com">Say hello <span>↗</span></a>
      </nav>
    </header>
  )
}

function IntroVideo() {
  return (
    <section className="intro-video" id="top" aria-label="Girafft on the water">
      {headerVideo && <video className="intro-video-media" autoPlay muted loop playsInline preload="metadata" aria-hidden="true">
        <source src={headerVideo} type="video/mp4" />
      </video>}
      <div className="intro-video-shade" />
      <div className="intro-video-title">
        <h1>GIRAFFT</h1>
        <p>a passion project by Justin Bartell</p>
      </div>
      <div className="intro-video-coordinate">32° 43′ 05″ N<br />117° 13′ 43″ W</div>
      <a className="intro-video-cue" href="#overview" aria-label="Continue to the project introduction">
        <span aria-hidden="true" />
      </a>
    </section>
  )
}

function Hero() {
  return (
    <section className="hero" id="overview">
      <div className="hero-copy">
        <p className="eyebrow"><span className="eyebrow-line" /> PERSONAL USV BUILD · SAN DIEGO, CALIFORNIA</p>
        <h1>A little boat.<br /><em>A lot of</em> figuring it out.</h1>
        <p className="hero-lede">Girafft is a USV passion project, outfitting a bare hull with propulsion, steering, electronics, and code to create a remotely controlled unmanned surface vessel.</p>
        <div className="hero-actions">
          <a className="button button--dark" href="#build">Follow the build <span>↓</span></a>
        </div>
        <div className="hero-meta">
          <span><b>01</b> / BUILDER'S LOG</span>
          <span>Manual control first. Autonomy someday.</span>
        </div>
      </div>
      <div className="hero-art">
        <div className="hero-image-wrap">
          <img src={headerPhoto} alt="Justin standing with the Girafft hull" />
          <div className="render-tag"><span className="render-spark">✳</span><span><b>THE BUILDER &amp; THE BOAT</b><small>Justin with the Girafft hull</small></span></div>
        </div>
      </div>
    </section>
  )
}

function BuildTimeline({ active, setActive }) {
  const stepRefs = useRef([])

  useEffect(() => {
    const nodes = stepRefs.current.filter(Boolean)
    if (!nodes.length) return

    let frame = 0
    const updateActive = () => {
      if (frame) return
      frame = window.requestAnimationFrame(() => {
        const anchor = window.innerHeight * 0.38
        const closest = nodes.reduce((best, node) => {
          const rect = node.getBoundingClientRect()
          const distance = rect.top <= anchor && rect.bottom >= anchor
            ? 0
            : Math.min(Math.abs(rect.top - anchor), Math.abs(rect.bottom - anchor))
          return distance < best.distance
            ? { index: Number(node.dataset.index), distance }
            : best
        }, { index: 0, distance: Infinity })
        setActive(closest.index)
        frame = 0
      })
    }

    window.addEventListener('scroll', updateActive, { passive: true })
    window.addEventListener('resize', updateActive)
    updateActive()
    return () => {
      window.removeEventListener('scroll', updateActive)
      window.removeEventListener('resize', updateActive)
      if (frame) window.cancelAnimationFrame(frame)
    }
  }, [setActive])

  const goToStep = (index) => {
    setActive(index)
    stepRefs.current[index]?.scrollIntoView({ behavior: 'smooth', block: 'center' })
  }

  return (
    <section className="build-section" id="build">
      <div className="section-heading">
        <p className="eyebrow"><span className="eyebrow-line" /> THE BUILD, BROKEN DOWN</p>
        <div className="heading-row"><h2>From floating shell<br />to <em>something that moves.</em></h2><p>Each chapter has its own carousel. Swipe, scroll, or use the arrows to browse the photos and videos.</p></div>
      </div>
      <div className="build-layout">
        <div className="timeline" aria-label="Girafft build timeline">
          <div className="timeline-rail" aria-hidden="true"><span style={{ height: `${((active + 1) / chapters.length) * 100}%` }} /></div>
          {chapters.map((chapter, index) => (
            <article
              className={`timeline-step ${active === index ? 'is-active' : ''}`}
              key={chapter.id}
              id={chapter.id}
              ref={(node) => { stepRefs.current[index] = node }}
              data-index={index}
            >
              <button className="step-marker" onClick={() => goToStep(index)} aria-label={`Jump to ${chapter.label}`}>{chapter.number}</button>
              <div className="step-content">
                <div className="step-kicker"><span>{chapter.label}</span><span className={`status-pill status-pill--${chapter.status.toLowerCase().replace(' ', '-')}`}>{chapter.status}</span></div>
                <h3>{chapter.title}</h3>
                {chapter.specs && <ul className="hull-specs">{chapter.specs.map((spec, specIndex) => <li key={specIndex}>{spec}</li>)}</ul>}
                <p>{chapter.copy}</p>
                {chapter.detail && <p>{chapter.detail}</p>}
                {chapter.note && <div className="step-note"><span className="note-icon">↳</span><span>{chapter.note}</span></div>}
                <div className="step-tag">{chapter.tag}</div>
              </div>
              <TimelineCarousel chapter={chapter} isActive={active === index} />
            </article>
          ))}
          <div className="timeline-end"><span className="end-spark">✳</span><span>More build notes as the boat evolves.</span></div>
        </div>
      </div>
    </section>
  )
}

function StatusCard() {
  return (
    <section className="status-section" id="next">
      <div className="status-main">
        <p className="eyebrow eyebrow--light"><span className="eyebrow-line" /> RIGHT NOW</p>
        <h2>MVP successful,<br />iterating.</h2>
        <p>Girafft succeeded in propulsion and steerage testing on the water. Immediate next steps include better waterproofing through hulls and increasing control range.</p>
        <div className="status-callout"><span className="status-orbit"><span /></span><span><b>First sea trial</b><small>Successful · November 8, 2026</small></span><span className="callout-arrow">↗</span></div>
      </div>
      <div className="status-side">
        <div className="side-topline"><span>THE NORTH STAR</span><span>01 / MVP</span></div>
        <div className="north-star-icon" aria-hidden="true"><span className="star-cross" /><span className="star-ring" /><span className="star-core" /></div>
        <h3>Reliable manual<br />RC control.</h3>
        <p>Autonomy is a future direction. First, the boat needs to reliably do what the person at the transmitter asks, from range, and without taking on water :D</p>
        <div className="side-bottom"><span>THROTTLE</span><i /> <span>STEERING</span><i /> <span>RANGE</span></div>
      </div>
    </section>
  )
}

function Roadmap() {
  const items = [
    ['01', 'Balance the hull', 'A keel is on the list for better balance.'],
    ['02', 'Tune the turn', 'More steering angle for a sharper turn radius.'],
    ['03', 'Stay out longer', 'Battery performance and solar charging.'],
    ['04', 'Make room up front', 'A front payload area with future drone capability.'],
    ['05', 'Bring data back', 'Remote feedback, sensors, and telemetry.'],
  ]
  return (
    <section className="roadmap-section" id="future">
      <div className="roadmap-intro"><p className="eyebrow"><span className="eyebrow-line" /> AFTER THE FIRST SPLASH</p><h2>Plenty of runway<br />left.</h2><p>The current target is an RC boat that works. These are ideas for the next laps around the bay.</p><div className="giraffe-note"><span>✳</span><p><b>And yes, the giraffe.</b><br />Paint, spots, and a head on that center pole—personality is part of the payload.</p></div><img className="giraffe-photo" src={`${base}images/hull/IMG_3751.jpeg`} alt="Girafft with a giraffe head mounted on the center pole" loading="lazy" /></div>
      <div className="roadmap-list">
        {items.map(([number, title, copy]) => <div className="roadmap-item" key={number}><span className="roadmap-number">{number}</span><div><h3>{title}</h3><p>{copy}</p></div><span className="roadmap-arrow">↗</span></div>)}
        <div className="autonomy-note"><span className="autonomy-mark">⌁</span><p><b>Long-term horizon</b><br />Sensors, telemetry, and eventually autonomy—after the manual boat is dependable.</p></div>
      </div>
    </section>
  )
}

function BuilderCarousel({ items }) {
  const trackRef = useRef(null)
  const videoRefs = useRef([])
  const [currentItem, setCurrentItem] = useState(0)

  const moveToItem = (index) => {
    if (!items.length) return
    const next = (index + items.length) % items.length
    setCurrentItem(next)
    const track = trackRef.current
    const item = track?.children[next]
    if (track && item) track.scrollTo({ left: item.offsetLeft - track.offsetLeft, behavior: 'smooth' })
  }

  useEffect(() => {
    videoRefs.current.forEach((video, index) => {
      if (!video) return
      video.muted = true
      video.volume = 0
      if (index === currentItem) video.play().catch(() => {})
      else video.pause()
    })
  }, [currentItem, items])

  return (
    <div className="builder-gallery">
      <div className="carousel-heading">
        <span>{String(currentItem + 1).padStart(2, '0')} / {String(items.length).padStart(2, '0')} <i>{items.length === 1 ? 'MEDIA ITEM' : 'MEDIA ITEMS'}</i></span>
        {items.length > 1 && <div className="carousel-arrows">
          <button type="button" onClick={() => moveToItem(currentItem - 1)} aria-label="Previous builder photo or video">←</button>
          <button type="button" onClick={() => moveToItem(currentItem + 1)} aria-label="Next builder photo or video">→</button>
        </div>}
      </div>
      <div
        className="carousel-track"
        ref={trackRef}
        onScroll={() => {
          const track = trackRef.current
          if (!track) return
          const nearest = [...track.children].reduce((best, item, index) => {
            const distance = Math.abs((item.offsetLeft - track.offsetLeft) - track.scrollLeft)
            return distance < best.distance ? { index, distance } : best
          }, { index: 0, distance: Infinity })
          setCurrentItem(nearest.index)
        }}
        role="region"
        aria-roledescription="carousel"
        aria-label="Photos and videos of Justin Bartell"
      >
        {items.map((item, index) => (
          <figure className="carousel-item" key={item.filename} aria-roledescription="slide">
            <div className="carousel-frame">
              {item.type === 'video' ? (
                <video ref={(node) => { videoRefs.current[index] = node }} muted loop playsInline preload="metadata" aria-label={item.caption}>
                  <source src={item.src} type={item.mimeType} />
                  Your browser does not support embedded video.
                </video>
              ) : <img src={item.src} alt={`Justin Bartell — ${item.caption}`} loading="lazy" />}
            </div>
            <figcaption><span>{item.type === 'video' ? 'VIDEO' : 'PHOTO'}</span><b>{item.caption}</b></figcaption>
          </figure>
        ))}
      </div>
      {items.length > 1 && <div className="carousel-dots" aria-label="Choose a builder media item">
        {items.map((item, index) => <button key={item.filename} type="button" className={currentItem === index ? 'is-active' : ''} onClick={() => moveToItem(index)} aria-label={`Show ${item.caption}`} aria-pressed={currentItem === index} />)}
      </div>}
    </div>
  )
}

function Builder() {

  return (
    <section className="builder-section" id="builder">
      <div className="builder-heading">
        <p className="eyebrow"><span className="eyebrow-line" /> THE BUILDER</p>
        <h2>Field-ready.<br /><em>Systems-minded.</em></h2>
        <p className="builder-intro">I’m a native San Diegan, program manager, and SQL engineer by trade. Girafft brings together years of technical delivery with the boating, diving, and safety experience I’ve built on the water.</p>
        <a className="button button--dark builder-resume" href={`${base}Justin-Bartell-Resume.pdf`} target="_blank" rel="noreferrer">Download resume <span>↓</span></a>
      </div>
      {builderMedia.length ? <BuilderCarousel items={builderMedia} /> : (
        <div className="builder-photo-placeholder"><span>JB</span><p>Photos of the builder<br />coming soon.</p></div>
      )}
      <div className="builder-credentials">
        <article className="builder-card">
          <p className="eyebrow">ON THE WATER</p>
          <ul>
            <li>San Diego Yacht Club member; frequently sails J/22s on San Diego Bay</li>
            <li>American Sailing Association 101, 103, 104, and 114</li>
            <li>Captained a 45-foot catamaran with seven guests on a week-long bareboat trip in Belize</li>
            <li>SSI Diver Stress &amp; Rescue; approximately 100 dives, including caves, wrecks, drift, low visibility, cold water, night dives, and depths to 120 feet</li>
          </ul>
        </article>
        <article className="builder-card">
          <p className="eyebrow">SAFETY &amp; READINESS</p>
          <ul>
            <li>American Red Cross First Aid/CPR/AED; has performed CPR during a cardiac arrest</li>
            <li>TWIC card holder and California Boater Card holder</li>
            <li>Maritime Institute Boater 202: Marine Electrical Systems</li>
            <li>STCW Basic Training scheduled for December 14–18, 2026</li>
          </ul>
        </article>
        <article className="builder-card">
          <p className="eyebrow">TECHNICAL SYSTEMS</p>
          <ul>
            <li>8+ years delivering software and data products, progressing from software and data engineering into program management</li>
            <li>Hands-on with SQL, PostgreSQL, Python, JavaScript, APIs, and data integrations</li>
            <li>Leads cross-functional work across engineering, vendors, QA, and customer teams; turns needs into technical plans, working systems, and training</li>
          </ul>
        </article>
      </div>
    </section>
  )
}

function Footer() {
  return (
    <footer className="site-footer">
      <div className="footer-top"><a className="wordmark wordmark--footer" href="#top"><span className="wordmark-mark"><span /></span><span>GIRAFFT<small>BUILT BY JUSTIN BARTELL</small></span></a><p>Girafft<br /><em>Learning through electrocution</em></p><div className="footer-links"><a href="https://www.linkedin.com/in/justin-bartell-000/" target="_blank" rel="noreferrer">LinkedIn <span>↗</span></a><a href="mailto:justin@bartell.com">Email <span>↗</span></a></div></div>
      <div className="footer-bottom"><span>© 2026 JUSTIN BARTELL · A WORK IN PROGRESS, LIKE THE BOAT</span><a href="#top">BACK TO TOP ↑</a></div>
    </footer>
  )
}

function App() {
  const [active, setActive] = useState(0)
  return <><Header /><main><IntroVideo /><Hero /><BuildTimeline active={active} setActive={setActive} /><StatusCard /><Roadmap /><Builder /></main><Footer /></>
}

createRoot(document.getElementById('root')).render(<App />)
