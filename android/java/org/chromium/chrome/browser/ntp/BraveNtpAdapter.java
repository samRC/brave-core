/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.ntp;

import android.app.Activity;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.util.Pair;
import android.os.Build;
import android.graphics.Bitmap;
import android.widget.FrameLayout;
import android.widget.ProgressBar;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import androidx.core.widget.ImageViewCompat;
import android.content.res.ColorStateList;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.bumptech.glide.RequestManager;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

import org.chromium.base.ContextUtils;
import org.chromium.brave_news.mojom.BraveNewsController;
import org.chromium.chrome.R;
import org.chromium.base.Log;
import org.chromium.chrome.browser.app.BraveActivity;
import org.chromium.chrome.browser.QRCodeShareDialogFragment;
import org.chromium.chrome.browser.night_mode.GlobalNightModeStateProviderHolder;
import org.chromium.chrome.browser.util.BraveConstants;
import org.chromium.chrome.browser.brave_news.BraveNewsUtils;
import org.chromium.chrome.browser.brave_news.models.FeedItemsCard;
import org.chromium.chrome.browser.brave_stats.BraveStatsUtil;
import org.chromium.chrome.browser.ntp_background_images.util.NTPUtil;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.chrome.browser.preferences.BravePref;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.ntp_background_images.model.SponsoredTab;
import org.chromium.chrome.browser.ntp_background_images.model.BackgroundImage;
import org.chromium.chrome.browser.ntp_background_images.NTPBackgroundImagesBridge;
import org.chromium.chrome.browser.ntp_background_images.model.NTPImage;
import org.chromium.chrome.browser.ntp_background_images.model.Wallpaper;
import org.chromium.chrome.browser.util.TabUtils;
import org.chromium.chrome.browser.preferences.BravePrefServiceBridge;
import org.chromium.chrome.browser.settings.BraveNewsPreferences;

import java.util.concurrent.CopyOnWriteArrayList;
import java.util.List;

public class BraveNtpAdapter
        extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    private Activity activity;
    private RequestManager glide;
    private BraveNewsController braveNewsController;
    private View siteSectionView;
    private CopyOnWriteArrayList<FeedItemsCard> newsItems;
    private NTPImage ntpImage;
    private SponsoredTab sponsoredTab;
    private Bitmap sponsoredLogo;
    private Wallpaper wallpaper;
    private NTPBackgroundImagesBridge nTPBackgroundImagesBridge;

    private static int TYPE_STATS = 1;
    private static int TYPE_TOP_SITES = 2;
    private static int TYPE_IMAGE_CREDIT = 3;
    private static int TYPE_NEWS_OPTIN = 4;
    private static int TYPE_NEWS_LOADING = 5;
    private static int TYPE_NEWS = 6;

    public BraveNtpAdapter(Activity activity, RequestManager glide,
                           CopyOnWriteArrayList<FeedItemsCard> newsItems,
                           BraveNewsController braveNewsController, View siteSectionView, 
                           NTPImage ntpImage, SponsoredTab sponsoredTab, Wallpaper wallpaper, 
                           Bitmap sponsoredLogo, NTPBackgroundImagesBridge nTPBackgroundImagesBridge) {

        this.activity = activity;
        this.newsItems = newsItems;
        this.braveNewsController = braveNewsController;
        this.glide = glide;
        this.siteSectionView = siteSectionView;
        this.ntpImage = ntpImage;
        this.sponsoredTab = sponsoredTab;
        this.wallpaper = wallpaper;
        this.sponsoredLogo = sponsoredLogo;
        this.nTPBackgroundImagesBridge = nTPBackgroundImagesBridge;
    }

    @Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder holder, int position) {
        if (holder instanceof StatsViewHolder) {
            StatsViewHolder statsViewHolder = (StatsViewHolder) holder;

            statsViewHolder.titleLayout.setVisibility(View.GONE);
            List<Pair<String, String>> statsPairs = BraveStatsUtil.getStatsPairs();
            
            statsViewHolder.adsBlockedCountTv.setText(statsPairs.get(0).first);
            statsViewHolder.dataSavedValueTv.setText(statsPairs.get(1).first);
            statsViewHolder.estTimeSavedCountTv.setText(statsPairs.get(2).first);
            statsViewHolder.adsBlockedCountTextTv.setText(statsPairs.get(0).second);
            statsViewHolder.dataSavedValueTextTv.setText(statsPairs.get(1).second);
            statsViewHolder.estTimeSavedCountTextTv.setText(statsPairs.get(2).second);

        } else if (holder instanceof TopSitesViewHolder) {
            siteSectionView.setBackgroundResource(R.drawable.rounded_dark_bg_alpha);

        } else if (holder instanceof ImageCreditViewHolder) {
            ImageCreditViewHolder imageCreditViewHolder = (ImageCreditViewHolder) holder;
            
            if (ntpImage instanceof Wallpaper
                && NTPUtil.isReferralEnabled()
                && Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            
                imageCreditViewHolder.superReferralLogo.setVisibility(View.VISIBLE);
                imageCreditViewHolder.creditTv.setVisibility(View.GONE);
                int floatingButtonIcon = R.drawable.ic_qr_code;
                imageCreditViewHolder.superReferralLogo.setImageResource(floatingButtonIcon);
                int floatingButtonIconColor =
                    GlobalNightModeStateProviderHolder.getInstance().isInNightMode()
                    ? android.R.color.white
                    : android.R.color.black;
                ImageViewCompat.setImageTintList(imageCreditViewHolder.superReferralLogo,
                    ColorStateList.valueOf(activity.getResources().getColor(floatingButtonIconColor)));
                imageCreditViewHolder.superReferralLogo.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        QRCodeShareDialogFragment qRCodeShareDialogFragment =
                            new QRCodeShareDialogFragment();
                        qRCodeShareDialogFragment.setQRCodeText(
                            BraveConstants.BRAVE_REF_URL + nTPBackgroundImagesBridge.getSuperReferralCode());
                        qRCodeShareDialogFragment.show(
                            ((BraveActivity) activity).getSupportFragmentManager(),
                            "QRCodeShareDialogFragment");
                    }
                });

            } else if (UserPrefs.get(Profile.getLastUsedRegularProfile()).getBoolean(
                       BravePref.NEW_TAB_PAGE_SHOW_BACKGROUND_IMAGE)
                   && sponsoredTab != null
                   && NTPUtil.shouldEnableNTPFeature()) {

                if (ntpImage instanceof BackgroundImage) {
                    BackgroundImage backgroundImage = (BackgroundImage) ntpImage;
                    imageCreditViewHolder.sponsoredLogo.setVisibility(View.GONE);
                    imageCreditViewHolder.superReferralLogo.setVisibility(View.GONE);

                    if (backgroundImage.getImageCredit() != null) {
                        String imageCreditStr = String.format(activity.getResources().getString(R.string.photo_by, backgroundImage.getImageCredit().getName()));

                        SpannableStringBuilder spannableString = new SpannableStringBuilder(imageCreditStr);
                        spannableString.setSpan(
                            new android.text.style.StyleSpan(android.graphics.Typeface.BOLD),
                            ((imageCreditStr.length() - 1)
                            - (backgroundImage.getImageCredit().getName().length() - 1)),
                            imageCreditStr.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);

                        imageCreditViewHolder.creditTv.setText(spannableString);
                        imageCreditViewHolder.creditTv.setVisibility(View.VISIBLE);

                        imageCreditViewHolder.creditTv.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View view) {
                                if (backgroundImage.getImageCredit() != null) {
                                    TabUtils.openUrlInSameTab(
                                        backgroundImage.getImageCredit().getUrl());
                                }
                            }
                        });
                    }
                }
            } 
            if(!NTPUtil.isReferralEnabled() && sponsoredLogo!=null) {
                imageCreditViewHolder.sponsoredLogo.setVisibility(View.VISIBLE);
                imageCreditViewHolder.sponsoredLogo.setImageBitmap(sponsoredLogo);
                imageCreditViewHolder.sponsoredLogo.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (wallpaper.getLogoDestinationUrl() != null) {
                            TabUtils.openUrlInSameTab(wallpaper.getLogoDestinationUrl());
                            nTPBackgroundImagesBridge.wallpaperLogoClicked(wallpaper);
                        }
                    }
                });
            }
        } else if(holder instanceof NewsOptinViewHolder) {
            NewsOptinViewHolder newsOptinViewHolder = (NewsOptinViewHolder) holder;

            newsOptinViewHolder.optinClose.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    //TODO: optin close
                }
            });

            newsOptinViewHolder.optinLearnMore.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    TabUtils.openUrlInSameTab(BraveConstants.BRAVE_LEARN_MORE_URL);
                }
            });

            newsOptinViewHolder.optinButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    newsOptinViewHolder.optinButton.setClickable(false);
                    newsOptinViewHolder.optinTv.setVisibility(View.INVISIBLE);
                    newsOptinViewHolder.optinLoadingSpinner.setVisibility(View.VISIBLE);
                    //TODO: next things
                }
            });
        } else if(holder instanceof NewsLoadingViewHolder) {
            NewsLoadingViewHolder newsLoadingViewHolder = (NewsLoadingViewHolder) holder;

        } else if(holder instanceof NewsViewHolder) {
            NewsViewHolder newsViewHolder = (NewsViewHolder) holder;

        }
    }

    @Override
    public int getItemCount() {
        if(isNewsOptin() || isNewsLoading()) {
            return 4;
        } else if(shouldDisplayNews()){
            return 3+newsItems.size();
        } else {
            return 3;
        }
    }

    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view;
        if (viewType == TYPE_STATS) {
            view = LayoutInflater.from(parent.getContext()).inflate(R.layout.brave_stats_layout, parent, false);
            return new StatsViewHolder(view);

        } else if (viewType == TYPE_TOP_SITES) {
            return new TopSitesViewHolder(siteSectionView);

        } else if (viewType == TYPE_IMAGE_CREDIT) {
            view = LayoutInflater.from(parent.getContext()).inflate(R.layout.ntp_image_credit, parent, false);
            return new ImageCreditViewHolder(view);

        } else if (viewType == TYPE_NEWS_OPTIN) {
            view = LayoutInflater.from(parent.getContext()).inflate(R.layout.optin_layout, parent, false);
            return new NewsOptinViewHolder(view);

        } else if (viewType == TYPE_NEWS_LOADING) {
            view = LayoutInflater.from(parent.getContext()).inflate(R.layout.news_loading, parent, false);
            return new NewsLoadingViewHolder(view);

        } else {
            view = LayoutInflater.from(parent.getContext()).inflate(R.layout.brave_news_row, parent, false);
            return new NewsViewHolder(view);
        }
    }

    @Override
    public int getItemViewType(int position) {
        if (position == 0) {
            return TYPE_STATS;
        } else if (position == 1) {
            return TYPE_TOP_SITES;
        } else if (position == 2) {
            return TYPE_IMAGE_CREDIT;
        } else if (position == 3 && isNewsOptin()) {
            return TYPE_NEWS_OPTIN;
        } else if (position == 3 && isNewsLoading()) {
            return TYPE_NEWS_LOADING;
        } else {
            return TYPE_NEWS;
        }
    }

    private boolean isNewsOptin() {
        
        boolean isNewsOn = BravePrefServiceBridge.getInstance().getNewsOptIn();
        boolean isShowOptin = ContextUtils.getAppSharedPreferences().getBoolean(BraveNewsPreferences.PREF_SHOW_OPTIN, true);
        boolean isShowNewsOn = BravePrefServiceBridge.getInstance().getShowNews();
        Log.e("tapan","isNewsOn:"+isNewsOn+",isShowOptin:"+isShowOptin+",isShowNewsOn:"+isShowNewsOn);
        return (!isShowNewsOn && isShowOptin) || (isNewsOn && isShowOptin);
    }

    private boolean isNewsLoading() {
        return BravePrefServiceBridge.getInstance().getShowNews() && BravePrefServiceBridge.getInstance().getNewsOptIn();
    }

    private boolean shouldDisplayNews() {

        return BravePrefServiceBridge.getInstance().getShowNews();
    }
    
    public void setSponsoredLogo(Wallpaper wallpaper, Bitmap sponsoredLogo) {

        this.wallpaper = wallpaper;
        this.sponsoredLogo = sponsoredLogo;
        notifyItemChanged(2);
        //notifyDataSetChanged();
    }

    public void setNtpImage(NTPImage ntpImage) {

        this.ntpImage = ntpImage;
        notifyItemChanged(2);
        //notifyDataSetChanged();
    }

    public static class StatsViewHolder extends RecyclerView.ViewHolder {
        LinearLayout titleLayout;
        TextView adsBlockedCountTv;
        TextView adsBlockedCountTextTv;
        TextView dataSavedValueTv;
        TextView dataSavedValueTextTv;
        TextView estTimeSavedCountTv;
        TextView estTimeSavedCountTextTv;

        StatsViewHolder(View itemView) {
            super(itemView);
            this.titleLayout = (LinearLayout) itemView.findViewById(R.id.brave_stats_title_layout);
            this.adsBlockedCountTv = (TextView) itemView.findViewById(R.id.brave_stats_text_ads_count);
            this.adsBlockedCountTextTv = (TextView) itemView.findViewById(R.id.brave_stats_text_ads_count_text);
            this.dataSavedValueTv = (TextView) itemView.findViewById(R.id.brave_stats_data_saved_value);
            this.dataSavedValueTextTv = (TextView) itemView.findViewById(R.id.brave_stats_data_saved_value_text);
            this.estTimeSavedCountTv = (TextView) itemView.findViewById(R.id.brave_stats_text_time_count);
            this.estTimeSavedCountTextTv = (TextView) itemView.findViewById(R.id.brave_stats_text_time_count_text);
        }
    }

    public static class TopSitesViewHolder extends RecyclerView.ViewHolder {
        
        TopSitesViewHolder(View itemView) {
            super(itemView);
        }
    }

    public static class ImageCreditViewHolder extends RecyclerView.ViewHolder {
        FrameLayout imageCreditLayout;
        FloatingActionButton superReferralLogo;
        TextView creditTv;
        ImageView sponsoredLogo;

        ImageCreditViewHolder(View itemView) {
            super(itemView);
            this.imageCreditLayout = (FrameLayout) itemView.findViewById(R.id.image_credit_layout);
            this.superReferralLogo = (FloatingActionButton) itemView.findViewById(R.id.super_referral_logo);
            this.creditTv = (TextView) itemView.findViewById(R.id.credit_text);
            this.sponsoredLogo = (ImageView) itemView.findViewById(R.id.sponsored_logo);
        }
    }

    public static class NewsOptinViewHolder extends RecyclerView.ViewHolder {
        FrameLayout optinButton;
        ProgressBar optinLoadingSpinner;
        ImageView optinClose;
        TextView optinLearnMore;
        TextView optinTv;

        NewsOptinViewHolder(View itemView) {
            super(itemView);
            optinButton = (FrameLayout) itemView.findViewById(R.id.optin_button);
            optinClose = (ImageView) itemView.findViewById(R.id.close_optin);
            optinLearnMore = (TextView) itemView.findViewById(R.id.optin_learnmore);
            optinTv = (TextView) itemView.findViewById(R.id.optin_button_text);
            optinLoadingSpinner = (ProgressBar) itemView.findViewById(R.id.optin_loading_spinner);
        }
    }

    public static class NewsLoadingViewHolder extends RecyclerView.ViewHolder {
        LinearLayout linearLayout;

        NewsLoadingViewHolder(View itemView) {
            super(itemView);
            this.linearLayout = (LinearLayout) itemView.findViewById(R.id.card_layout);
        }
    }

    public static class NewsViewHolder extends RecyclerView.ViewHolder {
        LinearLayout linearLayout;

        NewsViewHolder(View itemView) {
            super(itemView);
            this.linearLayout = (LinearLayout) itemView.findViewById(R.id.card_layout);
        }
    }
}
