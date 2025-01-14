/**
 * Bio component that queries for data
 * with Gatsby's useStaticQuery component
 *
 * See: https://www.gatsbyjs.org/docs/use-static-query/
 */

import React from "react"
import { useStaticQuery, graphql } from "gatsby"
import Image from "gatsby-image"
import styled from 'styled-components'
import { rhythm } from "../utils/typography"

const Wrapper = styled.section`
  display: flex;
  margin-bottom: 1 rem;
`;

const Bio = () => {
  const data = useStaticQuery(graphql`
    query BioQuery {
      avatar: file(absolutePath: { regex: "/gc-profile.jpg/" }) {
        childImageSharp {
          fixed(width: 50, height: 50) {
            ...GatsbyImageSharpFixed
          }
        }
      }
      site {
        siteMetadata {
          author
          social {
            instagram
          }
        }
      }
    }
  `)

  const { author, social } = data.site.siteMetadata
  return (
    <Wrapper>
      <Image
        fixed={data.avatar.childImageSharp.fixed}
        alt={author}
        style={{
          marginRight: rhythm(1 / 2),
          marginBottom: 0,
          minWidth: 50,
          borderRadius: `100%`,
        }}
        imgStyle={{
          borderRadius: `50%`,
        }}
      />
      <p>
        Written by <strong>{author}</strong> who lives and works wherever the
        mood strikes him.
        {` `}
        <a href={`https://instagram.com/${social.instagram}`}>
          He would be honored if you would follow him on Instagram
        </a>
      </p>
    </Wrapper>
  )
}

export default Bio
