; Color headings
(h1) @markup.heading.1.markdown
(h2) @markup.heading.2.markdown
(h3) @markup.heading.3.markdown
(h4) @markup.heading.4.markdown
(h5) @markup.heading.5.markdown
(h6) @markup.heading.6.markdown

; Color links
(link) @markup.link

; Conceal typical punctuation characters
([
 (h1_open)
 (h2_open)
 (h3_open)
 (h4_open)
 (h4_open)
 (h5_open)
 (link_alias_separator)
  ] @conceal (#set! conceal ""))

; Conceal link openers and closers, if they are part of an intact link
(link
   ((link_open) @conceal (#set! conceal ""))
   ((link_close) @conceal (#set! conceal "")))

; Conceal link address, if there is an alias
(link
   ((link_address) @conceal (#set! conceal ""))
   (link_alias_separator))
