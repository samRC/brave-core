import * as React from "react";
import { forwardRef, useCallback } from "react";
import styled from "styled-components";

function PageIndicator() {
    return <svg viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
        <circle cx="50" cy="50" r="50" />
    </svg>
}

const StyledButton = styled('button')`
    all: unset;

    cursor: pointer;

    color: var(--brave-palette-white);
    opacity: 0.5;

    :hover {
        opacity: 0.8
    }

    width: var(--list-page-button-size);
    height: var(--list-page-button-size);
`;

interface GridPageButtonProps {
    page: number;
    pageContainerRef: React.MutableRefObject<HTMLDivElement | undefined>;
}

export default function GridPageButton(props: GridPageButtonProps) {
    const updatePage = useCallback(() => props
        .pageContainerRef
        .current
        ?.children[props.page]
        .scrollIntoView({ behavior: 'smooth' }),
        [props.page, props.pageContainerRef]);

    return <StyledButton onClick={updatePage}>
        <PageIndicator />
    </StyledButton>
}



export const GridPageIndicatorContainer = styled('div') <{}>`
  position: absolute;
  color: var(--brave-palette-white);

  width: var(--list-page-button-size);
  height: var(--list-page-button-size);
`;

export const GridPageIndicator = forwardRef<HTMLDivElement>((props, ref) =>
    <GridPageIndicatorContainer ref={ref}>
        <PageIndicator />
    </GridPageIndicatorContainer>);
