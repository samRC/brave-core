import styled from 'styled-components';

export const GridPagesContainer = styled('div') <{}>`
  display: flex;
  flex-direction: row;

  padding: 24px 24px 0 24px;
  max-width: 596px;
  overflow-x: auto;

  scroll-snap-type: x mandatory;

  ::-webkit-scrollbar {
    display: none;
  }
`;
