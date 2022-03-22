// Copyright (c) 2020 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

import {
  SettingsRow,
  SettingsText,
  StyledCustomBackgroundOption,
  StyledCustomBackgroundOptionImage,
  StyledCustomBackgroundOptionLabel,
  StyledCustomBackgroundOptionSolidColor,
  StyledCustomBackgroundSettings,
  StyledUploadIconContainer,
  StyledUploadLabel
} from '../../../components/default'
import braveBackground from './assets/brave-background.png'
import UploadIcon from './assets/upload-icon'
import { Toggle } from '../../../components/toggle'

import { getLocale } from '../../../../common/locale'

import SolidColorChooser from './solidColorChooser'

interface Props {
  toggleBrandedWallpaperOptIn: () => void
  toggleShowBackgroundImage: () => void
  useCustomBackgroundImage: (useCustom: boolean) => void
  useSolidColorBackground: (color: string) => void
  brandedWallpaperOptIn: boolean
  showBackgroundImage: boolean
  featureCustomBackgroundEnabled: boolean
}

interface State {
  location: string
}

class BackgroundImageSettings extends React.PureComponent<Props, State> {
  constructor (props: Props) {
    super(props)
    this.state = {
      location: ''
    }
  }

  setLocation = (location: string) => {
    this.setState({ location })
  }

  onClickCustomBackground = () => {
    this.props.useCustomBackgroundImage(true)
  }

  onClickBraveBackground = () => {
    this.props.useCustomBackgroundImage(false)
  }

  onClickSolidColorBackground = () => {
    this.setState({ location: 'solidColors' })
  }

  render () {
    const {
      toggleShowBackgroundImage,
      toggleBrandedWallpaperOptIn,
      brandedWallpaperOptIn,
      showBackgroundImage,
      featureCustomBackgroundEnabled
    } = this.props

    return (
      <>
        {this.state.location === '' && (
          <div>
            <SettingsRow>
              <SettingsText>{getLocale('showBackgroundImage')}</SettingsText>
              <Toggle
                onChange={toggleShowBackgroundImage}
                checked={showBackgroundImage}
                size='large'
              />
            </SettingsRow>
            <SettingsRow isChildSetting={true}>
              <SettingsText>{getLocale('brandedWallpaperOptIn')}</SettingsText>
              <Toggle
                onChange={toggleBrandedWallpaperOptIn}
                // This option can only be enabled if
                // users opt in for background images
                checked={showBackgroundImage && brandedWallpaperOptIn}
                disabled={!showBackgroundImage}
                size='small'
              />
            </SettingsRow>
            {showBackgroundImage && featureCustomBackgroundEnabled && (
              <StyledCustomBackgroundSettings>
                <StyledCustomBackgroundOption
                  onClick={this.onClickCustomBackground}
                >
                  <StyledUploadIconContainer>
                    <UploadIcon />
                    <StyledUploadLabel>
                      {getLocale('customBackgroundImageOptionUploadLabel')}
                    </StyledUploadLabel>
                  </StyledUploadIconContainer>
                  <StyledCustomBackgroundOptionLabel>
                    {getLocale('customBackgroundImageOptionTitle')}
                  </StyledCustomBackgroundOptionLabel>
                </StyledCustomBackgroundOption>
                <StyledCustomBackgroundOption
                  onClick={this.onClickBraveBackground}
                >
                  <StyledCustomBackgroundOptionImage src={braveBackground} />
                  <StyledCustomBackgroundOptionLabel>
                    {getLocale('braveBackgroundImageOptionTitle')}
                  </StyledCustomBackgroundOptionLabel>
                </StyledCustomBackgroundOption>
                <StyledCustomBackgroundOption
                  onClick={this.onClickSolidColorBackground}
                >
                  <StyledCustomBackgroundOptionSolidColor
                    style={{ backgroundColor: '#151E9A' }}
                  />
                  <StyledCustomBackgroundOptionLabel>
                    { getLocale('solidColorTitle') }
                  </StyledCustomBackgroundOptionLabel>
                </StyledCustomBackgroundOption>
              </StyledCustomBackgroundSettings>
            )}
          </div>
        )}
        {this.state.location === 'solidColors' && (
          <SolidColorChooser
            useSolidColorBackground={this.props.useSolidColorBackground}
            setLocation={this.setLocation}
          />
        )}
      </>
    )
  }
}

export default BackgroundImageSettings
