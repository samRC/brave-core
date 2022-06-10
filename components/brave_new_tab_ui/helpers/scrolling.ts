import * as React from "react";

const overflowScrollableRegex = /(auto)|(scroll)/g;
const isScrollable = (element: Element) => {
    return (element.scrollWidth > element.clientWidth
        || element.scrollHeight > element.clientHeight)
        && overflowScrollableRegex.test(getComputedStyle(element).overflow);
}

export const getScrollableParents = (element: Element | null | undefined) => {
    const scrollableElements: Element[] = [];

    while (element) {
        if (isScrollable(element))
            scrollableElements.push(element);
        element = element.parentElement;
    }

    return scrollableElements;
}

export const useParentScrolled = (ref: React.MutableRefObject<HTMLElement | undefined>, handler: (e: Event) => void) => {
    React.useEffect(() => {
        const scrollables = getScrollableParents(ref.current);
        for (const scrollable of scrollables)
            scrollable.addEventListener('scroll', handler);
        return () => {
            for (const scrollable of scrollables)
                scrollable.removeEventListener('scroll', handler);
        }
    });
}
