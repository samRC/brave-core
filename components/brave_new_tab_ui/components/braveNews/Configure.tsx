import * as React from "react";
import styled from "styled-components";
import Flex from "../Flex";
import Discover from "./Discover";
import { BackArrow, Cross } from "./Icons";
import Button from "$web-components/button";
import Toggle from '$web-components/toggle';
import FeedList from "./FeedList";
import { useState } from "react";
import DisabledPlaceholder from "./DisabledPlaceholder";

const Grid = styled.div`
  width: 100%;
  height: 100%;

  display: grid;
  grid-template-columns: 250px auto;
  grid-template-rows: 64px 2px auto;

  grid-template-areas:
    "back-button header"
    "separator separator"
    "sidebar content";
`;

const Header = styled(Flex)`
  grid-area: header;
  padding: 12px;
`;

const HeaderText = styled.span`
  font-size: 16px;
  font-weight: 500;
`;

const CloseButton = styled(Button)`
  --inner-border-size: 0;
  --outer-border-size: 0;
  padding: 12px;
  width: 32px;
  height: 32px;
`

const BackButtonContainer = styled.div`
  grid-area: back-button;
  align-items: center;
  display: flex;
  padding: 12px;
`;

const BackButton = styled(Button)`
  --inner-border-size: 0;
  --outer-border-size: 0;
`;

const BackButtonText = styled.span`
  font-size: 12px;
  line-height: 1;
`;

const Hr = styled.hr`
  grid-area: separator;
  width: 100%;
  align-self: center;
`;

const Sidebar = styled.div`
  position: relative;
  grid-area: sidebar;
  padding: 12px;
`;

// Overlay on top of the sidebar, shown when it is disabled.
const SidebarOverlay = styled.div`
  position: absolute;
  top: 0;
  bottom: 0;
  left: 0;
  right: 0;
  background: white;
  opacity: 0.7;
`

const Content = styled.div`
  grid-area: content;
  padding: 12px;
`;

export default function Configure() {
  const [enabled, setEnabled] = useState(true);

  return (
    <Grid>
      <BackButtonContainer>
        <BackButton onClick={console.log}>
          {BackArrow}
          <BackButtonText>
            Back to <b>Dashboard</b>
          </BackButtonText>
        </BackButton>
      </BackButtonContainer>
      <Header direction="row-reverse" gap={12} align="center" justify="space-between">
        <CloseButton onClick={console.log}>{Cross}</CloseButton>
        {enabled && <Flex direction="row" align="center" gap={8}>
          <HeaderText>Brave News</HeaderText>
          <Toggle isOn={enabled} onChange={setEnabled} />
        </Flex>}
      </Header>
      <Hr />
      <Sidebar>
        <FeedList />
        {!enabled && <SidebarOverlay />}
      </Sidebar>
      <Content>
        {enabled
          ? <Discover />
          : <DisabledPlaceholder enableBraveNews={() => setEnabled(true)} />}
      </Content>
    </Grid>
  );
}
