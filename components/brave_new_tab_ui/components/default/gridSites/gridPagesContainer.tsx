import styled from 'styled-components';

export const GridPagesContainer = styled('div') <{}>`
  display: flex;
  flex-direction: row;

  padding: 24px 24px 0 24px;
  max-width: calc((var(--grid-columns) + 1) * var(--grid-column-width));
  overflow-x: auto;

  scroll-snap-type: x mandatory;

  @media screen and (max-width: 870px) {
    margin-left: var(--grid-column-width);
  }

  ::-webkit-scrollbar {
    display: none;
  }
`;
